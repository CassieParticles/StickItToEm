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
	playButton = guiManager->createButton({ -64,-120 }, { 512,1024 }, { 128,64 }, { 1,1,1 }, [levelManager]() {levelManager->setLevel(1); });


}

MainMenu::~MainMenu()
{
}

void MainMenu::openLevel()
{
	BaseLevel::openLevel();
	playButton->setDraw(true);
}

void MainMenu::closeLevel()
{
	playButton->setDraw(false);
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


