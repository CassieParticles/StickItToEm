#include "GameLevel.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#include <iostream>

#include "../Engine/Timer.h"
#include "../Engine/Input.h"
#include "../Engine/Program.h"

#include "TerrainManager.h"
#include "Player.h"

GameLevel::GameLevel(GLFWwindow* window,Input* input,  glm::vec4 bgColour):BaseLevel(window,input,bgColour),player{input,glm::ivec2{60,35},{ 10,20 },50}
{
	terrainManager = new TerrainManager({ 60,35 });	//Create the terrain manager

	terrainManager->uploadStage(stageValues);	//59x34, the scalar values used to generate the marchingSquares

	player.setTerrainManager(terrainManager);

	glGenBuffers(1, &terrainUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, terrainUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::ivec2), &(terrainManager->getArenaSize()), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, terrainUBO);

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
	if(input->getKeyDown(GLFW_KEY_T))
	{
		terrainManager->modifyTerrainCircle(player.getPosition(), 5, -0.2);
	}
	if (input->getKeyDown(GLFW_KEY_Y))
	{
		terrainManager->modifyTerrainCircle(player.getPosition(), 5, 0.2);
	}

	player.handleInput(updateTimer->getDeltaTime());
}

void GameLevel::update(Timer* updateTimer)
{
	player.collisionResolution(updateTimer->getDeltaTime());
	player.update(updateTimer->getDeltaTime());
}

void GameLevel::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	terrainManager->render();
	
	player.render();
	
	endDraw();
}