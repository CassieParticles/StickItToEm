#include "GameLevel.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <iostream>

#include "../Engine/Timer.h"
#include "../Engine/Input.h"
#include "../Engine/Program.h"

GameLevel::GameLevel(GLFWwindow* window,Input* input,  glm::vec4 bgColour):BaseLevel(window,input,bgColour)
{
	program = new Program{ "src/Shaders/testShader/vertex.glsl","src/Shaders/testShader/fragment.glsl"};	//Create test program

	glGenVertexArrays(1, &vaoID);	//Create test mesh
	glGenBuffers(1, &vboID);//

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);
}

GameLevel::~GameLevel()
{
	delete program;

	glDeleteBuffers(1, &vboID);
	glDeleteVertexArrays(1, &vaoID);
}

void GameLevel::openLevel()
{
	BaseLevel::openLevel();	//Call the base function used in opening level before doing other fancy stuff
}

void GameLevel::closeLevel()
{

}

void GameLevel::handleInput(Timer* updateTimer)
{
	input->update();
	if(input->getKeyPressed(GLFW_KEY_W))
	{
		std::cout << "Key W pressed\n";
	}
}

void GameLevel::update(Timer* updateTimer)
{

}

void GameLevel::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	program->use();
	program->setVec3("colour", { 1.f,0.f,0.f });

	glBindVertexArray(vaoID);
	glDrawArrays(GL_TRIANGLES, 0,3);
	
	
	endDraw();
}