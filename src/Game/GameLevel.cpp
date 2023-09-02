#include "GameLevel.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#include <iostream>

#include <Engine/Timer.h>
#include <Engine/Input.h>
#include <Engine/Program.h>

#include <Engine/GUI.h>

#include "TerrainManager.h"
#include "Player.h"
#include "Weapon.h"

GameLevel::GameLevel(GLFWwindow* window,Input* input,GUIManager* guiManager,  glm::vec4 bgColour):BaseLevel(window,input,guiManager,bgColour),player1{input,{ 10,20 },50,{1,0,0}},player2{input,{40,20},50,{0,0,1}}
{
	terrainManager = new TerrainManager({ 60,35 });	//Create the terrain manager

	terrainManager->uploadStage(stageValues);	//59x34, the scalar values used to generate the marchingSquares

	player1.setTerrainManager(terrainManager);
	player2.setTerrainManager(terrainManager);

	player1.setInputs({ GLFW_KEY_A,GLFW_KEY_D,GLFW_KEY_W,GLFW_KEY_S,GLFW_KEY_Q,GLFW_KEY_E });
	player2.setInputs({ GLFW_KEY_J,GLFW_KEY_L,GLFW_KEY_I,GLFW_KEY_K,GLFW_KEY_U,GLFW_KEY_O });

	glGenBuffers(1, &terrainUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, terrainUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::ivec2), &(terrainManager->getArenaSize()), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, terrainUBO);

	weapon = new Weapon({ 10,15 }, Weapon::rocketLauncher);
}

GameLevel::~GameLevel()
{
	delete terrainManager;

	delete weapon;
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
	glm::vec2 mousePos = input->getMousePositionNormalised();
	mousePos.y = 1 - mousePos.y;
	if(input->getKeyDown(GLFW_KEY_T))
	{
		terrainManager->modifyTerrainCircle(mousePos * glm::vec2(terrainManager->getArenaSize()), 5, -0.2);
	}
	if (input->getKeyDown(GLFW_KEY_Y))
	{
		terrainManager->modifyTerrainCircle(mousePos * glm::vec2(terrainManager->getArenaSize()), 5, 0.2);
	}

	player1.handleInput(updateTimer->getDeltaTime());
	player2.handleInput(updateTimer->getDeltaTime());
}

void GameLevel::update(Timer* updateTimer)
{
	player1.collisionResolution(updateTimer->getDeltaTime());
	player2.collisionResolution(updateTimer->getDeltaTime());

	player1.update(updateTimer->getDeltaTime());
	player2.update(updateTimer->getDeltaTime());

	guiManager->update();
}

void GameLevel::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	terrainManager->render();

	player1.render();
	player2.render();

	weapon->render();

	guiManager->render();
	
	endDraw();
}