#include "MainMenu.h"

#include <iostream>
#include <functional>

#include <Engine/LevelManager.h>
#include <Engine/GUI.h>
#include <Engine/Input.h>
#include <glfw3.h>

void func()
{

}

MainMenu::MainMenu(Input* input, GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour) :BaseLevel{ input,guiManager,levelManager,bgColour }
{

	buttonFont = guiManager->createFont("assets/fonts/Roboto-Black.ttf", 32, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM ");

	playButton = guiManager->createButton({ -64,-120 }, { 512,1024 }, { 128,64 }, { 1,1,1 }, [levelManager]() {levelManager->setLevel(1); },this);

	playText = guiManager->createText({ -64,-120 }, { 512,1024 }, { 1,1 }, "Play game", buttonFont, { 0,0,0 },-1,-1,this);
}

MainMenu::~MainMenu()
{
}

void MainMenu::openLevel()
{
	BaseLevel::openLevel();
}

void MainMenu::closeLevel()
{
	BaseLevel::closeLevel();
}

void MainMenu::handleInput(Timer* updateTimer)
{
	input->update();
}

void MainMenu::update(Timer* updateTimer)
{
	guiManager->update();
}

void MainMenu::render(Timer* frameTimer)
{
	beginDraw();

	guiManager->render();

	endDraw();
}


