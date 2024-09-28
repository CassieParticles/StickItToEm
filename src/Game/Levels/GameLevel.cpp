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
#include <Engine/LevelManager.h>

#include "../TerrainManager.h"
#include "../Player.h"
#include "../WeaponManager.h"
#include "../Gunsmoke.h"

GameLevel::GameLevel(Input* input,GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour):BaseLevel(input,guiManager,levelManager,bgColour),player1{input,{ 10,10 },50,{1,0,0}},player2{input,{40,10},50,{0,0,1}}
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

	testFont = guiManager->createFont("assets/fonts/Roboto-Black.ttf", 48, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM:. 1234567890%");

	player1Health = guiManager->createText({ -300,64 }, { 512,0 }, { 1,1 }, "0%", testFont, glm::vec3(1, 0, 0),-1,-1,this);
	player2Health = guiManager->createText({ 300,64 }, { 512,0 }, { 1,1 }, "0%", testFont, glm::vec3(0, 0, 1),-1,-1,this);

	winScreenText = guiManager->createText({ -150,40 }, { 512,512 }, {2,2},"Winnder",testFont,glm::vec3(1,1,1),-1,-1,this);

	winScreenText->setDraw(false);

	player1OOBTimer = 0;
	player2OOBTimer = 0;



	//testRect = guiManager->createTextureRect(glm::vec2(64, 64), glm::vec2(512, 512), glm::vec2(128, 128), "assets/troll.jpg", glm::vec3(1, 1, 1));

	weaponSpawnTimer = new Timer(0.5f);
}

GameLevel::~GameLevel()
{
	delete terrainManager;

	delete weaponManager;

	delete gunSmokeManager;

	delete weaponSpawnTimer;
}

void GameLevel::openLevel()
{
	player1.reset();
	player2.reset();
	weaponManager->deleteAllWeapons();
	BaseLevel::openLevel();	//Call the base function used in opening level before doing other fancy stuff
	winScreenTimer = 0;
}

void GameLevel::closeLevel()
{
	BaseLevel::closeLevel();
	winScreenText->setDraw(false);
}

void GameLevel::handleInput(Timer* updateTimer)
{
	input->update();
	glm::vec2 mousePos = input->getMousePositionNormalised();
	mousePos.y = 1 - mousePos.y;

	player1.handleInput(updateTimer->getDeltaTime());
	player2.handleInput(updateTimer->getDeltaTime());
}

void GameLevel::update(Timer* updateTimer)
{
	player1.collisionResolution(updateTimer->getDeltaTime());
	player2.collisionResolution(updateTimer->getDeltaTime());

	player1.update(updateTimer->getDeltaTime());
	player2.update(updateTimer->getDeltaTime());

	//Player 1 out of bounds increment timer
	if (player1.getPosition().x < 0 || player1.getPosition().x>terrainManager->getArenaSize().x ||
		player1.getPosition().y < 0 || player1.getPosition().y>terrainManager->getArenaSize().y)
	{
		player1OOBTimer += updateTimer->getDeltaTime();
	}
	else
	{
		player1OOBTimer = 0;
	}

	if (player2.getPosition().x < 0 || player2.getPosition().x>terrainManager->getArenaSize().x ||
		player2.getPosition().y < 0 || player2.getPosition().y>terrainManager->getArenaSize().y)
	{
		std::cout << player2.getPosition().x<< ":" << player2.getPosition().y << '\n';
		std::cout << terrainManager->getArenaSize().x << ":" << terrainManager->getArenaSize().y << '\n';
		player2OOBTimer += updateTimer->getDeltaTime();
	}
	else
	{
		player2OOBTimer = 0;
	}

	if (player1OOBTimer > OOBLoseTime)
	{
		player2OOBTimer = 0;
		winScreenText->setDraw(true);
		winScreenText->setColour({ 0,0,1 });
		winScreenText->generateNewString("Player 2 won");
		winScreenTimer+= updateTimer->getDeltaTime();
	}
	else if (player2OOBTimer > OOBLoseTime)
	{
		player1OOBTimer = 0;
		winScreenText->setDraw(true);
		winScreenText->setColour({ 1,0,0 });
		winScreenText->generateNewString("Player 1 won");
		winScreenTimer+= updateTimer->getDeltaTime();
	}

	if (winScreenTimer > 3.f)
	{
		levelManager->setLevel(0);
	}

	weaponManager->update(updateTimer->getDeltaTime());

	std::string player1Str = std::to_string(static_cast<int>(player1.getDamage())) + "%";
	std::string player2Str = std::to_string(static_cast<int>(player2.getDamage())) + "%";

	player1Health->generateNewString(player1Str);
	player2Health->generateNewString(player2Str);

	weaponSpawnTimer->Update();
	if (weaponSpawnTimer->getUpdate())
	{
		float spawnX=rand()/static_cast<float>(rand.max()) * (spawnXMax-spawnXMin) + spawnXMin;
		glm::vec2 spawnLoc = { spawnX,spawnY };

		weaponManager->spawnRandomWeapon(spawnLoc, { WeaponType::rocketLauncher,WeaponType::shotgun });
	}

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

	guiManager->render();
	
	endDraw();
}