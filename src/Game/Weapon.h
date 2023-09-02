#pragma once
#include <glm.hpp>

#include "Engine/Collision.h"

class Player;
class Animation;
class Program;

class TerrainManager;

class Weapon
{
public:
	enum WeaponType
	{
		rocketLauncher
	};

	Weapon(glm::vec2 position,  WeaponType type, TerrainManager* terrainManager);
	~Weapon();

	rect getCollisionRect();

	void update(float deltaTime);
	void render();

	void setWielder(Player* wielder) { this->wielder = wielder; }
	Player* getWielder() { return wielder; }

	friend bool checkPlayerWeapon(Player* player);
protected:
	glm::vec2 position;
	glm::vec2 size;

	glm::vec2 gravityVel{0,-9.8f};		//I'm lazy so the weapon falls at a fixed rate

	WeaponType type;

	Player* wielder=nullptr;
	TerrainManager* terrain;

	unsigned int vaoID;	//Vertex data
	unsigned int vertexBuffers[3];

	Program* weaponProgram;

	unsigned int textureID;
	Animation* idleAnimation;
};