#include <iostream>
#include <functional>
#include <fstream>

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#include <OpenALSoft/al.h>
#include <OpenALSoft/alc.h>
#include <AudioFile.h>

#include <Engine/Timer.h>
#include <Engine/Input.h>
#include <Engine/TextureManager.h>
#include <Engine/Window.h>
#include <Engine/GUI/GUIManager.h>
#include <Engine/FileManager.h>
#include <Engine/LevelManager.h>

#include "Game/Levels/GameLevel.h"

int main()
{
	Window::init(1200,700,3,3,"Stick it to 'em");

	//Create the window
	GLFWwindow* window{};
	
	window = Window::getWindow();

	Timer updateTimer{ 60 };
	Timer renderTimer{ 60 };
	Input input{ };

	GUIManager guiManager{&input};

	LevelManager levelManager{};	//Create the level manager

	GameLevel gameLevel{&input,&guiManager,&levelManager,{.196f, .254f, .467f,1.f}};	//Initialise the levels

	levelManager.addLevel(&gameLevel);	//Add the levels

	levelManager.setLevel(0);

	while (!glfwWindowShouldClose(window))
	{
		updateTimer.Update();
		renderTimer.Update();
		
		if (updateTimer.getUpdate())	//Update the game
		{
			levelManager.handleInput(&updateTimer);
			levelManager.update(&updateTimer);
		}
		if (renderTimer.getUpdate())
		{
			levelManager.render(&renderTimer);
		}
		if (input.getKeyDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	TextureManager::cleanup();

	return 0;
}