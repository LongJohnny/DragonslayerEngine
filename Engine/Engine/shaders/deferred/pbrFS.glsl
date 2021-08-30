#version 330 core

layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform vec3 lightPosition = { 0.0, 0.0, 10.0};
uniform vec3 viewPosition;

uniform vec3 lightColor = {150.0, 150.0, 150.0};

uniform sampler2D gBufferPositionMetallic; // Contains both the position and metallic values
uniform sampler2D gBufferNormalRoughness; // Contains both the normal and roughness values
uniform sampler2D gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values
uniform sampler2D shadowMap;

const float PI = 3.1415927;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 pbr(vec3 position, vec3 normal, vec3 albedo, float metallic, float roughness, float ambientOcclusion) {
    vec3 V = normalize(viewPosition - position);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // calculate per-light radiance
    vec3 L = normalize(lightPosition - position);
    vec3 H = normalize(V + L);
    float distance = length(lightPosition - position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, H, roughness);   
    float G   = GeometrySmith(normal, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = numerator / denominator;
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(normal, L), 0.0);        

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ambientOcclusion;
    
    vec3 color = ambient + Lo;

    return color;
}

float calculateShadows(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
	// Perspective division
	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w; // Actually meaningless since the proj matrix used for light space is orthographic
	// Moving the coords to [0,1] which is the range of the depth map
	projCoords = projCoords * 0.5 + 0.5;
	// Closest depth from the light's point of view
	float closestDepth = texture(shadowMap, projCoords.xy).r;   
	// Current depth from the light's point of view
	float currentDepth = projCoords.z;  

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// PCF Filter
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(currentDepth > 1.0)
        return 0.0f;

	return shadow;
}

void main(void)
{

    // 1. Extract position, metallic map, normal, roughness map, albedo and ambient occlusion map values
    // from each of the buffers that compose the gBuffer
    vec4 positionMetallic = texture(gBufferPositionMetallic, fragTextCoords);
    vec4 normalRoughness = texture(gBufferNormalRoughness, fragTextCoords);
    vec4 albedoAmbientOcclusion = texture(gBufferAlbedoAmbientOcclusion, fragTextCoords);
    
    vec3 position = positionMetallic.rgb; 
    float metallic = positionMetallic.a;
    vec3 normal = normalRoughness.rgb; 
    float roughness = normalRoughness.a;
    vec3 albedo = albedoAmbientOcclusion.rgb; 
    float ambientOcclusion = albedoAmbientOcclusion.a;

    float shadow = texture(shadowMap, vec2(1,1)).r;  

    // 2. Calculate color using PBR
    vec3 color = pbr(position, normal, albedo, metallic, roughness, ambientOcclusion);
   
    // 3. Combine with shadow map
    // TODO

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    

    fragmentColor = vec4(color, 1.0);
}