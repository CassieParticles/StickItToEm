#include "Controls.h"

#include <Engine/Input.h>
#include <Engine/GUI.h>
#include <Engine/levelManager.h>

Controls::Controls(Input* input, GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour):BaseLevel{input, guiManager, levelManager, bgColour}
{
	controlsGUI = guiManager->createTextureRect({ -639,-374 }, { 512,512 }, { 1278 ,749 }, "assets/TutorialGUI.png", { 1,1,1 }, this);

	guiFont = guiManager->createFont("assets/fonts/Roboto-Black.ttf", 32, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM ");

	controlsButton = guiManager->createButton({ -90,100 }, { 512,0 }, { 190,64 }, { 1,1,1 }, [levelManager]() {levelManager->setLevel(0); }, this);
	buttonText = guiManager->createText({ -75,115 }, { 512,0 }, { 1,1 }, "Main menu", guiFont, { 0,0,0 }, -1, -1, this);
}

Controls::~Controls()
{
}

void Controls::openLevel()
{
	BaseLevel::openLevel();
}

void Controls::closeLevel()
{
	BaseLevel::closeLevel();
}

void Controls::handleInput(Timer* updateTimer)
{
	input->update();
}

void Controls::update(Timer* updateTimer)
{
	guiManager->update();
}


void Controls::render(Timer* frameTimer)
{
	BaseLevel::beginDraw();

	guiManager->render();

	BaseLevel::endDraw();
}
