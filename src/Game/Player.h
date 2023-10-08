#pragma once
#include <glm.hpp>

#include <Engine/Collision.h>
#include "../EngineAdditions/PlayerCollision.h"


class Weapon;
class Program;
class Input;
class TerrainManager;
class Animation;

class Player
{
public:

	enum playerAnimations
	{
		idle,
		walk,
		jump,
		fall
	};

	struct controls
	{
		int moveLeft;
		int moveRight;
		int jump;
		int shoot;
		int aimUp;
		int aimDown;
	};

	Player(Input* input,glm::vec2 position, float mass,glm::vec3 colour);
	~Player();

	void setTerrainManager(TerrainManager* terrainManager) { this->terrainManager = terrainManager; }

	void handleInput(float deltaTime);

	void collisionResolution(float deltaTime);

	void update(float deltaTime);

	void render();

	void setInputs(controls c) { con = c; }

	glm::vec2 getPosition() { return position; }

	glm::vec2 getVelocity() { return velocity; }
	float getMass() { return mass; }
	rect getCollisionRect();

	glm::vec2 getSize() { return playerSize; }

	void addForce(glm::vec2 force);
	void addForce(float x, float y) { addForce(glm::vec2{x, y}); }

	friend void Collision::resolvePlayerLine(Player* player, line* l, float deltaTime);

	void changeAnimation(playerAnimations newAnimation);
	bool getFlipped() { return flipped; }

	void setWeapon(Weapon* weapon);
	Weapon* getWeapon() { return weapon; }

	float getAngle() { return aimAngle; }	//Get the angle without it being flipped by the player direction
	float getAimAngle();

	void takeDamage(float damage) { this->damage+=damage; }			//To deal with the player taking damage
	void takeKnockback(glm::vec2 knockback);

protected:
	Input* input;
	TerrainManager* terrainManager;

	glm::vec2 playerSize{1.6f, 3.2f};	//Size of the player (in squares)

	glm::vec2 position{};
	glm::vec2 velocity{};	//Variables used in physics calculations
	float mass{};
	glm::vec2 sumForce{};

	glm::vec3 colour;

	bool grounded;	//If the player is on the ground
	glm::vec2 forward{};

	controls con{};	//Stored controls

	//Information needed to render player
	unsigned int vaoID;

	unsigned int vertexBuffers[3];

	Program* playerProgram;

	Animation* currentAnimation;	//Points to whichever animation is currently being used

	Animation* playerIdleAnim;
	Animation* playerWalkAnim;
	Animation* playerJumpAnim;
	Animation* playerFallAnim;

	bool flipped{};		//If the player is facing left or right

	Weapon* weapon;
	float aimAngle{};

	float damage{};

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

	static constexpr float playerAimSpeed{ 30.f * 3.14 * 0.005555f };	//Rotational speed of player aim, converted to radians

};