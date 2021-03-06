#include "DragonslayerEngine/Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assert.h>
#include "DragonslayerEngine/utils/OpenGLUtils.h"
#include <LMath/Vec2.hpp>
#include <LMath/Vec3.hpp>
#include <fstream>
#include "DragonslayerEngine/textures/FloatArrayFile.h"

using namespace LMath;

Texture2D::Texture2D() { }

Texture2D::Texture2D(const std::string& textureFilePath) {
	// Loading image data from textureFilePath
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	// If there is an error while reading the data from the image file this will be NULL
	assert(data != NULL);

	// Generating the texture
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	internalFormat = format;
	this->format = format;
	precision = GL_UNSIGNED_BYTE;
	
	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//free image data, no longer necessary
	stbi_image_free(data);
}

Texture2D::Texture2D(const std::string& textureFilePath, GLint param)
{
	// Loading image data from textureFilePath
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	// If there is an error while reading the data from the image file this will be NULL
	assert(data != NULL);

	// Generating the texture
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	internalFormat = format;
	this->format = format;
	precision = GL_UNSIGNED_BYTE;

	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//free image data, no longer necessary
	stbi_image_free(data);
}


void Texture2D::_deleteObject()
{
	GL_CALL(glDeleteTextures(1, &id));
}

Texture2D::~Texture2D() {
	id = 0u;
}

Texture2D* Texture2D::emptyTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format, GLenum type)
{
	// Texture must must be at least 1x1
	assert(width > 0 && height > 0);

	Texture2D* emptyTexture = new Texture2D();
	emptyTexture->width = width;
	emptyTexture->height = height;

	// Generating the texture
	GL_CALL(glGenTextures(1, &emptyTexture->id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, emptyTexture->id));

	// Empty texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL));
	emptyTexture->internalFormat = internalFormat;
	emptyTexture->format = format;
	emptyTexture->precision = type;

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	return emptyTexture;
}

Texture2D* Texture2D::depthTexture(unsigned int width, unsigned int height, GLenum type)
{
	// Texture must must be at least 1x1
	assert(width > 0 && height > 0);

	Texture2D* emptyTexture = new Texture2D();
	emptyTexture->width = width;
	emptyTexture->height = height;

	// Generating the texture
	GL_CALL(glGenTextures(1, &emptyTexture->id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, emptyTexture->id));

	// Empty texture
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, type, NULL));
	emptyTexture->internalFormat = GL_DEPTH_COMPONENT;
	emptyTexture->format = GL_DEPTH_COMPONENT;
	emptyTexture->precision = type;

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GL_CALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	return emptyTexture;
}

Texture2D Texture2D::fromFloatArrayFile(const std::string& textureFilePath, unsigned int width, unsigned int height)
{
	float* data = fa::loadFromFile(textureFilePath, 3 * width * height);

	Texture2D texture;

	// Generating the texture
	GL_CALL(glGenTextures(1, &texture.id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.id));

	texture.internalFormat = GL_RGB16F;
	texture.format = GL_RGB;
	texture.precision = GL_FLOAT;

	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	delete[] data;

	return texture;
}

void Texture2D::bind(unsigned int unitNumber) {
	if (unitNumber > 0u) {
		GL_CALL(glActiveTexture(unitNumber));
	}
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture2D::unBind(unsigned int unitNumber) {
	if (unitNumber > 0u) {
		GL_CALL(glActiveTexture(unitNumber));
	}
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture2D::getWidth() const
{
	return width;
}

int Texture2D::getHeight() const
{
	return height;
}

void Texture2D::resize(unsigned int width, unsigned int height)
{
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, precision, NULL);
	unBind();

	this->width = width;
	this->height = height;
}



