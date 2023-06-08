#include "Game.h"
#include "../Engine/Timer.h"

Game::Game(GLFWwindow* window, glm::vec4 bgColour):BaseLevel(window,bgColour)
{
}

void Game::openLevel()
{
	BaseLevel::openLevel();	//Call the base function used in opening level before doing other fancy stuff
}

void Game::closeLevel()
{

}

void Game::handleInput(Timer* updateTimer)
{
	
}

void Game::update(Timer* updateTimer)
{

}

void Game::render(Timer* frameTimer)
{
	beginDraw();
	//Draw stuff
	
	endDraw();
}