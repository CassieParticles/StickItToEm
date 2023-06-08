#include "BaseLevel.h"

#include <glad/glad.h>
#include <glfw3.h>

#include "Input.h"

BaseLevel::BaseLevel(GLFWwindow* window, Input* input, glm::vec4 bgColour) :window{ window }, input{ input },bgColour{ bgColour }
{
}

void BaseLevel::openLevel() 
{
	glClearColor(bgColour.r,bgColour.g,bgColour.b,bgColour.a);
}

void BaseLevel::closeLevel() {}

BaseLevel::~BaseLevel() {}

void BaseLevel::beginDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BaseLevel::endDraw()
{
	glfwSwapBuffers(window);	//Check and call events
}
