#pragma once

#include <Engine/BaseLevel.h>
#include <Engine/GUIFD.h>

class Controls : public BaseLevel
{
public:
	Controls(Input* input, GUIManager* guiManager, LevelManager* levelManager, glm::vec4 bgColour);
	~Controls();

	void openLevel() override;
	void closeLevel() override;

	void handleInput(Timer* updateTimer) override;
	void update(Timer* updateTimer) override;
	void render(Timer* frameTimer) override;
protected:
	GUITextureRect* controlsGUI;

	Font* guiFont;

	GUIButton* controlsButton;
	GUIText* buttonText;
};
