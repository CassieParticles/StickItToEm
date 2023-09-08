#include "BulletManager.h"

#include <Engine/Program.h>

#include "Bullet.h"
#include "TerrainManager.h"

BulletManager::BulletManager(std::vector<Player*>* players,TerrainManager* terrain):players{players}, terrain{ terrain }
{
	bulletProgram = new Program{ "src/Shaders/bullet/bullet.vert","src/Shaders/bullet/bullet.frag",Program::filePath };
}

BulletManager::~BulletManager()
{
	delete bulletProgram;

	for(int i=0;i<bullets.size();i++)
	{
		delete bullets.at(i);
	}
}

void BulletManager::addBullet(glm::vec2 position, float angle, BulletType type)
{
	float playerDamage;
	float terrainDamage;
	float areaRadius;

	switch(type)
	{
	case BulletType::rocket:
		playerDamage = rocketPlayerDamage;
		terrainDamage = rocketTerrainDamage;
		areaRadius = rocketDamageRadius;
		break;
	}

	Bullet* bullet = new Bullet{ position,angle,playerDamage,terrainDamage,areaRadius,type,bulletProgram };
	bullets.push_back(bullet);
}

void BulletManager::update(float deltaTime)
{
	for(int i=0;i<bullets.size();i++)
	{
		bullets.at(i)->update(deltaTime);
	}
}

void BulletManager::render()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->render();
	}
}
