#pragma once

#include <GLAD/glad.h>

class Gunsmoke
{
public:
	Gunsmoke();
	~Gunsmoke();

	unsigned int getTexture() { return texture; }
private:
	unsigned int texture;
};