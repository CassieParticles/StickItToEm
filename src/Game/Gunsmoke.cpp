#include "Gunsmoke.h"

#include <GLAD/glad.h>
#include <glfw3.h>

#include <Engine/Program.h>
#include <Engine/Window.h>
#include <Engine/GameObjects/GameObjects.h>

Gunsmoke::Gunsmoke()
{
	glGenTextures(2, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);	//Generate background texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	GameObjects::genTextureSquare(&vao, vbos + 0, vbos + 1);

	gunSmokeProgram = new Program{ "src/Shaders/gunSmoke/gunSmoke.vert","src/Shaders/gunSmoke/gunSmoke.frag" ,Program::filePath };

	gunSmokeProgram->setUniformBufferBlockBinding("terrainData", 1);
}


Gunsmoke::~Gunsmoke()
{
	glDeleteTextures(1, &texture);

	glDeleteFramebuffers(1, &frameBuffer);
}

void Gunsmoke::drawSmoke(glm::vec2 position, float radius, glm::vec3 colour)
{
	if (radius == 0) { return; }	//Don't accidentally divide by 0 in a shader

	glViewport(0, 0, 1024, 1024);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	gunSmokeProgram->use();

	gunSmokeProgram->setVec2("position",position);	//Set position, radius and colour of the gunSmoke
	gunSmokeProgram->setFloat("radius", radius);

	gunSmokeProgram->setVec3("colour", colour);

	glBindVertexArray(vao);	//Bind the VAO and element array buffer of the square
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//Draw the gunsmoke

	glBindVertexArray(0); // Unbind the VAO and element array buffer of the square
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, Window::getWidth(), Window::getHeight());
}

