#pragma once

#include <string>
#include <glm.hpp>
#include <vector>

#include "TextureManager.h"

class Program;

struct frame
{
	glm::vec2 tl;
	glm::vec2 tr;
	glm::vec2 bl;
	glm::vec2 br;
};

class Animation
{
public:
	Animation(const std::string& spriteSheetPath, int frameCount, Program* animationProgram);
	~Animation();

	void setFrameTime(float frameTime) { this->frameTime = frameTime; }	//Set the time in seconds it takes to change frame

	void update(float deltaTime);

	void setCurrentFrame();	//Gets the current frame of animation, packed into a 4x2 matrix so it can be passed in as a matrix

	bool getFlipped() { return flipped; }
	void setFlipped(bool flipped) { this->flipped = flipped; }
	void flipAnim() { flipped = 1 - flipped; }

protected:
	int flipped{};

	float sumTime{};

	float frameTime{1};

	Program* animationProgram;

	texture spriteSheet;

	std::vector<frame> frames;
	int frameCount;
};