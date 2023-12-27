#include "GameLevel.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <glfw3.h>
#include <iostream>
#include <chrono>

#include <Engine/Timer.h>
#include <Engine/Input.h>
#include <Engine/GUI.h>
#include <Engine/Window.h>

#include "../TerrainManager.h"
#include "../Player.h"
#include "../WeaponManager.h"
#include "../Gunsmoke.h"

GameLevel::GameLevel(Input* input,GUIManager* guiManager,  glm::vec4 bgColour):BaseLevel(input,guiManager,bgColour),player1{input,{ 10,25 },50,{1,0,0}},player2{input,{40,20},50,{0,0,1}}
{
	rand.seed(time(0));

	gunSmokeManager = new Gunsmoke();

	terrainManager = new TerrainManager({ 60,35 },gunSmokeManager);	//Create the terrain manager

	terrainManager->uploadStage(stageValues);	//59x34, the scalar values used to generate the marchingSquares

	player1.setTerrainManager(terrainManager);
	player2.setTerrainManager(terrainManager);

	player1.setInputs({ GLFW_KEY_A,GLFW_KEY_D,GLFW_KEY_W,GLFW_KEY_S,GLFW_KEY_Q,GLFW_KEY_E });
	player2.setInputs({ GLFW_KEY_J,GLFW_KEY_L,GLFW_KEY_I,GLFW_KEY_K,GLFW_KEY_U,GLFW_KEY_O });

	glGenBuffers(1, &terrainUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, terrainUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::ivec2), &(terrainManager->getArenaSize()), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, terrainUBO);

	weaponManager = new WeaponManager(terrainManager,gunSmokeManager,&rand);
	weaponManager->addPlayer(&player1);
	weaponManager->addPlayer(&player2);

	weaponManager->createWeapon(WeaponType::shotgun, { 20,20 });
	//weaponManager->createWeapon(WeaponType::rocketLauncher, { 40,20 });

	testFont = guiManager->createFont("assets/fonts/BreeSerif-Regular.ttf", 48, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM:. 1234567890");
	testText = guiManager->createText(glm::vec2(32, 32), glm::vec2(0, 0), glm::vec2(1, 1), "DeltaTime: 3.3", testFont, glm::vec3(1, 1, 1));

	//testRect = guiManager->createTextureRect(glm::vec2(64, 64), glm::vec2(512, 512), glm::vec2(128, 128), "assets/troll.jpg", glm::vec3(1, 1, 1));
}

GameLevel::~GameLevel()
{
	delete terrainManager;

	delete weaponManager;

	delete gunSmokeManager;
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
	if(input->getKeyPressed(GLFW_KEY_T))
	{
		terrainManager->modifyTerrainCircle(mousePos * glm::vec2(terrainManager->getArenaSize()), 5, -4);
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

	weaponManager->update(updateTimer->getDeltaTime());

	guiManager->update();
}

void GameLevel::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	terrainManager->render();

	player1.render();
	player2.render();

	weaponManager->render();

	testText->generateNewString("deltaTime: " +std::to_string(frameTimer->getDeltaTime()));

	guiManager->render();
	
	endDraw();
}