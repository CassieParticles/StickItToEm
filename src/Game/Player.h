#pragma once
#include <glm.hpp>


struct controls
{
	int moveLeft;
	int moveRight;
	int jump;
	int shoot;
	int aimUp;
	int aimDown;
};

struct rect
{
	glm::ivec2 tlCorner;
	glm::ivec2 brCorner;
};

class Program;

class Player
{
public:
	Player(glm::ivec2 gridSize);
	~Player();

	void handleInput(float deltaTime);

	void update(float deltaTime);

	void render();

	glm::vec2 position;
protected:


	glm::vec2 playerSize{1.6f, 3.2f};	//Size of the player (in squares)

	//Information needed to render player, shared across all instances
	unsigned int vaoID;			
	unsigned int vertVBO;

	Program* playerProgram;
};