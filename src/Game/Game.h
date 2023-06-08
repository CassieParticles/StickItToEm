#pragma once
#include "../Engine/BaseLevel.h"

class Program;

class Game :public BaseLevel
{
public:
	Game(GLFWwindow* window,Input* input, glm::vec4 bgColour);
	~Game();

	void openLevel() override;
	void closeLevel() override;

	void handleInput(Timer* updateTimer) override;
	void update(Timer* updateTimer) override;
	void render(Timer* frameTimer) override;
protected:
	//Temporary values, to test if this works
	unsigned int vaoID;
	unsigned int vboID;

	Program* program;

	glm::vec2 vertexPos[3] =
	{
		{ 0.0f, 0.4f},
		{ 0.5f,-0.5f},
		{-0.5f,-0.5f}
	};
};