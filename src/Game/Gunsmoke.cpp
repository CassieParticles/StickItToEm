#include "Gunsmoke.h"

#include <GLAD/glad.h>
#include <glfw3.h>

#include <Engine/Program.h>
#include <Engine/Window.h>
#include <Engine/GameObjects/GameObjects.h>

Gunsmoke::Gunsmoke()
{
	textures = new unsigned int[2];
	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	GameObjects::genTextureSquare(&vao, vbos + 0, vbos + 1);

	gunSmokeProgram = new Program{ "src/Shaders/gunSmoke/gunSmoke.vert","src/Shaders/gunSmoke/gunSmoke.frag" ,Program::filePath };

	gunSmokeProgram->setUniformBufferBlockBinding("terrainData", 1);
}


Gunsmoke::~Gunsmoke()
{
	glDeleteTextures(2, textures);

	glDeleteFramebuffers(1, &frameBuffer);

	delete[] textures;
}

void Gunsmoke::drawSmoke(glm::vec2 position, float radius, glm::vec3 colour)
{
	if (radius == 0) { return; }	//Don't accidentally divide by 0 in a shader

	glViewport(0, 0, 1024, 1024);

	unsigned int writeTexture = textures[1 - currentTexture];
	unsigned int readTexture = textures[currentTexture];

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, writeTexture,0);

	//Draw the read texture
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	
	gunSmokeProgram->use();	//Set up program
	gunSmokeProgram->setInt("tex", 0);

	gunSmokeProgram->setVec2("position",position);	//Set gunsmoke values
	gunSmokeProgram->setFloat("radius", radius);
	gunSmokeProgram->setVec3("colour", colour);

	glActiveTexture(GL_TEXTURE0);	//Bind the read texture
	glBindTexture(GL_TEXTURE_2D, readTexture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//Draw to write texture

	glUseProgram(0);	//Unbind current program

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);	//Unbind the write texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	currentTexture = 1 - currentTexture;	//Swap which texture is the most recent


	glViewport(0, 0, Window::getWidth(), Window::getHeight());
}

