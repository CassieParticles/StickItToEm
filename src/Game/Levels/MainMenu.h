#pragma once
#include <Engine/BaseLevel.h>

#include <Engine/GUIFD.h>

class MainMenu :public BaseLevel
{
public:
	MainMenu(Input* input, GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour);
	~MainMenu();

	void openLevel() override;
	void closeLevel() override;

	void handleInput(Timer* updateTimer) override;
	void update(Timer* updateTimer) override;
	void render(Timer* frameTimer) override;

protected:
	Font* buttonFont;

	GUIButton* playButton;
	GUIText* playText;
};