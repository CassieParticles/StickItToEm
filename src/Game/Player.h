#pragma once
#include <glm.hpp>

#include <Engine/Collision.h>
#include "../EngineAdditions/PlayerCollision.h"

struct controls
{
	int moveLeft;
	int moveRight;
	int jump;
	int shoot;
	int aimUp;
	int aimDown;
};


class Program;
class Input;
class TerrainManager;
class Animation;

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

	friend void Collision::resolvePlayerLine(Player* player, line* l, float deltaTime);

protected:
	Input* input;
	TerrainManager* terrainManager;

	glm::vec2 playerSize{1.6f, 3.2f};	//Size of the player (in squares)

	glm::vec2 position{};
	glm::vec2 velocity{};	//Variables used in physics calculations
	float mass{};
	glm::vec2 sumForce{};

	bool grounded;	//If the player is on the ground
	glm::vec2 forward{};

	//Information needed to render player, shared across all instances
	unsigned int vaoID;			
	unsigned int vertVBO;
	unsigned int cornerIndexVBO;

	Program* playerProgram;

	Animation* playerWalkAnim;

	//Constants involved in player
	static constexpr glm::vec2 gravForce{0, -19.6f};	
	static constexpr float terminalVelocity{ 20.f };

	static constexpr float frictionCoeffecient{ 0.4f };
	static constexpr float steepnessMax = .5f;
	static constexpr float playerJumpForce = { 25.f };	//Divided by deltatime so it's as if it's applied over 1 second

	static constexpr float playerRunForce{ 25.f };
	static constexpr float playerRunMaxSpeed{ 25.f };

	static constexpr float playerAirForce{ 12.f };
	static constexpr float playerAirMaxSpeed{ 15.f };

};