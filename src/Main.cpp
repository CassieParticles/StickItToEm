#include <iostream>

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/Timer.h"

int initWindow(GLFWwindow** window)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(800, 800, "Stick it to 'em", NULL, NULL);
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
	Timer renderTimer{ 60 };
	

	int err = initWindow(&window);
	if (err != 0) { return err; }

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		renderTimer.Update();
		if (renderTimer.getUpdate())
		{
			std::cout << renderTimer.getDeltaTime() << '\n';
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	return 0;
}