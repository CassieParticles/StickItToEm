#pragma once
#include "../Engine/BaseLevel.h"

class Program;
class TerrainManager;

class GameLevel :public BaseLevel
{
public:
	GameLevel(GLFWwindow* window,Input* input, glm::vec4 bgColour);
	~GameLevel();

	void openLevel() override;
	void closeLevel() override;

	void handleInput(Timer* updateTimer) override;
	void update(Timer* updateTimer) override;
	void render(Timer* frameTimer) override;
protected:
	float scalarField[20]	//5x4 grid of points, makes a 6x5 grid of squares (including ones on edge)
	{
		-1.f,-1.f, 1.f,-1.f,-1.f,
		-1.f,-1.f, 1.f,-.3f,-1.f,
		-1.f,-1.f, 1.f,-1.f,-1.f,
		 1.f, .4f, 1.f, 1.f, .7f,
	};

	TerrainManager* terrainManager;
};