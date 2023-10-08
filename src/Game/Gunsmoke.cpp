#include "Gunsmoke.h"

Gunsmoke::Gunsmoke()
{
	
	float* floatArray = new float[1024 * 1024 * 4];	//Create an array for the initial data of the texture

	for (int i = 0; i < 1024 * 1024; i++)
	{
		floatArray[i * 4 + 0] = 0.6f;
		floatArray[i * 4 + 1] = 0.0f;
		floatArray[i * 4 + 2] = 0.4f;
		floatArray[i * 4 + 3] = 0.5f;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_FLOAT, floatArray);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] floatArray;
}


Gunsmoke::~Gunsmoke()
{
	glDeleteTextures(1, &texture);
}