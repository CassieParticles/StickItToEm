#include "MainMenu.h"

#include <iostream>
#include <functional>

#include <Engine/LevelManager.h>
#include <Engine/GUI.h>
#include <Engine/Input.h>
#include <Engine/Window.h>

#include <glfw3.h>

void func()
{

}

MainMenu::MainMenu(Input* input, GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour) :BaseLevel{ input,guiManager,levelManager,bgColour }
{

	buttonFont = guiManager->createFont("assets/fonts/Roboto-Black.ttf", 32, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM ");

	playButton = guiManager->createButton({ -80,-120 }, { 512,1024 }, { 160,64 }, { 1,1,1 }, [levelManager]() {levelManager->setLevel(1); },this);
	playText = guiManager->createText({ -75,-100 }, { 512,1024 }, { 1,1 }, "Play game", buttonFont, { 0,0,0 },-1,-1,this);

	controlsButton = guiManager->createButton({ -80,-200 }, { 512,1024 }, { 160,64 }, { 1,1,1 }, [levelManager]() {levelManager->setLevel(2); }, this);
	controlsText = guiManager->createText({ -75,-180 }, { 512,1024 }, { 1,1 }, "Controls", buttonFont, { 0,0,0 }, -1, -1, this);

	quitButton = guiManager->createButton({ -80,-280 }, { 512,1024 }, { 160,64 }, { 1,1,1 }, []() {glfwSetWindowShouldClose(Window::getWindow(), true); },this);
	quitText = guiManager->createText({ -65,-260 }, { 512,1024 }, { 1,1 }, "Quit", buttonFont, { 0,0,0 }, -1, -1, this);

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


