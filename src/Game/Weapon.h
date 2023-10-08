#pragma once
#include <glm.hpp>

#include "Engine/Collision.h"

class Player;
class Animation;
class Program;

class TerrainManager;
class BulletManager;

enum class WeaponType
{
	rocketLauncher
};

class Weapon
{
public:

	Weapon(glm::vec2 position,  WeaponType type, TerrainManager* terrainManager,Program* weaponProgram,BulletManager* bulletManager);
	~Weapon();

	rect getCollisionRect();

	void update(float deltaTime);
	void render();

	void setWielder(Player* wielder) { this->wielder = wielder; }
	Player* getWielder() { return wielder; }

	void fireWeapon();

	int getAmmoLeft() { return ammoLeft; }

	bool getDelete() { return deleteFlag; }
	void setDelete() { deleteFlag = true; }
protected:
	glm::vec2 position;
	glm::vec2 size;

	float angle;

	glm::vec2 playerHandOffset;	//How much the weapon should be offset from the bottom right corner of the player

	glm::vec2 gravityVel{0,-9.8f};		//I'm lazy so the weapon falls at a fixed rate

	WeaponType type;

	Player* wielder=nullptr;
	TerrainManager* terrain;
	BulletManager* bulletManager;

	bool deleteFlag{ false };	//Flag for the weapon manager to know to delete this weapon

	unsigned int vaoID;	//Vertex data
	unsigned int vertexBuffers[3];

	Program* weaponProgram;

	unsigned int textureID;
	Animation* idleAnimation;

	int ammoLeft{};
};