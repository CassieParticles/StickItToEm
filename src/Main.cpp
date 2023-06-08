#include <iostream>

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/Timer.h"
#include "Engine/Input.h"

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
	int err = initWindow(&window);
	if (err != 0) { return err; }

	Timer renderTimer{ 60 };
	Input input{ window };

	while (!glfwWindowShouldClose(window))
	{
		renderTimer.Update();
		if (renderTimer.getUpdate())
		{
			input.update();
			if (input.getKeyDown(GLFW_KEY_ESCAPE))
			{
				glfwSetWindowShouldClose(window, true);
			}
		}
		
	}

	return 0;
}