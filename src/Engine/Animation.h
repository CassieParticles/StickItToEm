#pragma once

#include <string>
#include <glm.hpp>
#include <vector>

struct texture;

struct frame
{
	glm::vec2 tl;
	glm::vec2 tr;
	glm::vec2 br;
	glm::vec2 bl;
};

class Animation
{
public:
	Animation(const std::string& spriteSheetPath, int frameCount);
	~Animation();

	void update(float deltaTime);

	glm::mat4x2 getCurrentFrame();	//Gets the current frame of animation, packed into a 4x2 matrix so it can be passed in as a matrix

protected:
	float sumTime{};


	texture* spriteSheet;

	std::vector<frame> frames;
	int frameCount;
};