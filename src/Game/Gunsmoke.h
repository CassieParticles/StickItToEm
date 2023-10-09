#pragma once

#include <GLAD/glad.h>
#include <glm.hpp>

class Program;

class Gunsmoke
{
public:
	Gunsmoke();
	~Gunsmoke();

	unsigned int getTexture() { return textures[currentTexture]; }

	void drawSmoke(glm::vec2 position, float raadius, glm::vec3 colour);
private:
	unsigned int* textures;	//To get the texture to add smoke ontop of other smoke, it needs 2 textures and to swap between them

	unsigned int frameBuffer;

	//Vertex data for square for rendering read texture and for rendering gunsmoke
	unsigned int vao;
	unsigned int vbos[2];

	int currentTexture{};	//The write texture that will be used next, swapped each time new gunsmoke is drawn

	Program* gunSmokeProgram;
};
