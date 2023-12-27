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

	GameLevel gameLevel{&input,&guiManager,{.196f, .254f, .467f,1.f}};	//Set up the level for the game

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

	TextureManager::cleanup();

	return 0;
}