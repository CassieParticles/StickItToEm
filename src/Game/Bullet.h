#pragma once
#include <glm.hpp>

#include <Engine/Collision.h>

class BulletManager;
class Animation;
class Player;
class TerrainManager;
class Program;

enum class BulletType
{
	rocket
};

class Bullet
{
public:
	Bullet(glm::vec2 position, float angle, float playerDamage, float terrainDamage, float areaRadius,TerrainManager* terrain, BulletType type,Program* bulletProgram);
	~Bullet();

	void update(float deltaTime);
	void render();

	rect getCollisionRect();

	bool getDelete() { return deleteFlag; }
	void setDelete() { deleteFlag = true; }
protected:
	//Stuff for bullet dynamics
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;
	float angle;

	//Stuff for the bullet's mechanics
	TerrainManager* terrain;

	BulletType type;


	float playerDamage;
	float terrainDamage;
	float areaRadius;

	bool deleteFlag{false};	//Flag for the bulletManager to see if it should delete a bullet

	//Constants defined for the bullets
	static constexpr float rocketBulletSpeed=5.f;


	//Stuff for the bullet's rendering
	Program* bulletProgram;
	Animation* bulletAnim;
	unsigned int vaoID;	
	unsigned int vertexBuffers[3];
};