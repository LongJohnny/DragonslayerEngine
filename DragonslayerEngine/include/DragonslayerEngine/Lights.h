#ifndef LIGHTS_H
#define LIGHTS_H

#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "AreaLight.h"

namespace DragonslayerEngine {
	
	/*
	* Simple container of all the lights to be consired when rendering a frame
	*/
	struct Lights {

		std::vector<DirectionalLight> directionalLights;
		std::vector<PointLight> pointLights;
		std::vector<SpotLight> spotLights;
		std::vector<AreaLight> areaLights;
	};

}



#endif
