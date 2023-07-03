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
	glm::vec2 tlCorner;
	glm::vec2 size;

	float angle;	//Radians
};

class Program;
class Input;
class TerrainManager;

class Player
{
public:
	Player(Input* input,glm::ivec2 gridSize,glm::vec2 position, float mass);
	~Player();

	void setTerrainManager(TerrainManager* terrainManager) { this->terrainManager = terrainManager; }

	void handleInput(float deltaTime);

	void collisionResolution(float deltaTime);

	void update(float deltaTime);

	void render();

	glm::vec2 getPosition() { return position; }

	glm::vec2 getVelocity() { return velocity; }
	float getMass() { return mass; }
	rect getCollisionRect();

	void addForce(glm::vec2 force);
	void addForce(float x, float y) { addForce(glm::vec2{x, y}); }


protected:
	Input* input;
	TerrainManager* terrainManager;

	glm::vec2 playerSize{1.6f, 3.2f};	//Size of the player (in squares)

	glm::vec2 position{};
	glm::vec2 velocity{};	//Variables used in physics calculations
	float mass{};
	glm::vec2 sumForce{};

	//Information needed to render player, shared across all instances
	unsigned int vaoID;			
	unsigned int vertVBO;

	Program* playerProgram;

	static constexpr glm::vec2 gravForce{0, -9.8f};
	static constexpr float frictionCoeffecient{ 0.4f };
};