#pragma once
#include <glm.hpp>

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
	Bullet(glm::vec2 position, float angle, float playerDamage, float terrainDamage, float areaRadius,BulletType type,Program* bulletProgram);
	~Bullet();

	void update(float deltaTime);
	void render();

protected:
	//Stuff for bullet dynamics
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;
	float angle;

	//Stuff for the bullet's mechanics
	BulletType type;


	float playerDamage;
	float terrainDamage;
	float areaRadius;

	//Constants defined for the bullets
	static constexpr float rocketBulletSpeed=2.5f;


	//Stuff for the bullet's rendering
	Program* bulletProgram;
	Animation* bulletAnim;
	unsigned int vaoID;	
	unsigned int vertexBuffers[3];
};