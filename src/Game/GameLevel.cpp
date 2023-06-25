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

#include "TerrainManager.h"

GameLevel::GameLevel(GLFWwindow* window,Input* input,  glm::vec4 bgColour):BaseLevel(window,input,bgColour)
{
	terrainManager = new TerrainManager({ 60,35 });	//Create the terrain manager

	terrainManager->uploadStage(stageValues);	//59x34, the scalar values used to generate the marchingSquares


	//terrainManager->modifyTerrainCircle({ 40,30 }, 15, -4);
}

GameLevel::~GameLevel()
{
	delete terrainManager;
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
		terrainManager->modifyTerrainCircle({ 40,25 }, 5, -4);
	}
}

void GameLevel::update(Timer* updateTimer)
{

}

void GameLevel::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	terrainManager->render();
	
	
	endDraw();
}