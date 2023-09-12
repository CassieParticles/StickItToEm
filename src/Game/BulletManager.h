#pragma once

#include <vector>

#include "Bullet.h"

class TerrainManager;
class Player;
class Program;

class BulletManager
{
public:
	BulletManager(std::vector<Player*>* players, TerrainManager* terrain);
	~BulletManager();

	void addBullet(glm::vec2 position, float angle, BulletType type, Player* owner);
	void deleteBullet(int index);
	void deleteBullet(Bullet* bullet);

	void update(float deltaTime);
	void render();
protected:
	TerrainManager* terrain;	
	Program* bulletProgram;	//All bullets will share the same Program to save resources on bullet generation (kinda important)

	std::vector<Player*>* players;	//vectors to contain players and bullets
	std::vector<Bullet*> bullets;


	//Constants for bullets
	float rocketPlayerDamage = 60.f;
	float rocketTerrainDamage = 4.f;
	float rocketDamageRadius = 5.f;
};