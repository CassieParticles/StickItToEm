#pragma once
#include "../Engine/BaseLevel.h"

class Game :public BaseLevel
{
public:
	Game(GLFWwindow* window, glm::vec4 bgColour);

	void openLevel() override;
	void closeLevel() override;

	void handleInput(Timer* updateTimer) override;
	void update(Timer* updateTimer) override;
	void render(Timer* frameTimer) override;
protected:

};