#ifndef I_TEXTURE_H
#define I_TEXTURE_H

#include <iostream>
#include <glad/glad.h>

class ITexture {

protected:
	GLuint id = 0;

public:

	virtual void bind(unsigned int unitNumber) = 0;

	virtual void unBind(unsigned int unitNumber) = 0;

	GLuint getId() { return id; }
};

#endif
