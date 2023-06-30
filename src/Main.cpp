#include <iostream>

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/Timer.h"
#include "Engine/Input.h"

#include "Game/GameLevel.h"

int initWindow(GLFWwindow** window)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(1200, 700, "Stick it to 'em", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(*window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD";
		glfwTerminate();
		return 2;
	}


	glfwSetFramebufferSizeCallback(*window, [](GLFWwindow* window, int width, int height)->void {glViewport(0, 0, width, height); });

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "Window created!\n";
	return 0;
}

int main()
{	
	//Create the window
	GLFWwindow* window{};
	int err = initWindow(&window);
	if (err != 0) { return err; }

	Timer updateTimer{ 60 };
	Timer renderTimer{ 60 };
	Input input{ window };

	GameLevel gameLevel{window,&input,{.196f, .254f, .467f,1.f}};	//Set up the level for the game

	gameLevel.openLevel();	//Open the game level

	while (!glfwWindowShouldClose(window))
	{
		updateTimer.Update();
		renderTimer.Update();
		
		if (updateTimer.getUpdate())	//Update the game
		{
			gameLevel.handleInput(&updateTimer);
			gameLevel.update(&updateTimer);
		}
		if (renderTimer.getUpdate())
		{
			gameLevel.render(&renderTimer);
		}
		if (input.getKeyDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	return 0;
}