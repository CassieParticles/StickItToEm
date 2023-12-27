#pragma once
#include <glm.hpp>
#include <vector>

#include <Engine/Collision.h>

class BulletManager;
class Animation;
class Player;
class TerrainManager;
class Program;
class Gunsmoke;

enum class BulletType
{
	rocket,
	shotgunPellet
};

class Bullet
{
public:
	Bullet(glm::vec2 position, float angle,Player* playerFired,std::vector<Player*>* players,TerrainManager* terrain, BulletType type,Program* bulletProgram, Gunsmoke* gunSmokeManager);
	~Bullet();

	void update(float deltaTime);
	void render();

	rect getCollisionRect();
	rect getBoundingRect();

	bool getDelete() { return deleteFlag; }
	void setDelete() { deleteFlag = true; }

	glm::vec2 getCentre();
protected:
	//Stuff for bullet dynamics
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;
	float angle;

	//Stuff for the bullet's mechanics
	TerrainManager* terrain;
	Gunsmoke* gunSmokeManager;

	BulletType type;

	std::vector<Player*>* players;
	Player* playerFired;

	float playerDamage;
	float terrainDamage;
	float forceScalar;
	float areaRadius;

	bool deleteFlag{false};	//Flag for the bulletManager to see if it should delete a bullet

	//Constants defined for the bullets
	static constexpr float rocketBulletSpeed=15.f;
	static constexpr float rocketPlayerDamage = 60.f;
	static constexpr float rocketTerrainDamage = 4.f;
	static constexpr float rocketDamageRadius = 5.f;
	static constexpr float rocketScalarKnockback = 50000.f;

	static constexpr float shotgunBulletSpeed = 30.f;
	static constexpr float shotgunPlayerDamage = 15.f;
	static constexpr float shotgunTerrainDamage = 0.6f;
	static constexpr float shotgunDamageRadius = 1.5f;
	static constexpr float shotgunScalarKnockback = 1500.f;

	//Stuff for the bullet's rendering
	Program* bulletProgram;
	Animation* bulletAnim;
	unsigned int vaoID;	
	unsigned int vertexBuffers[3];
};