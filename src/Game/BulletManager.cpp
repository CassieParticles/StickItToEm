#include "BulletManager.h"

#include <iostream>
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

void BulletManager::addBullet(glm::vec2 position, float angle, BulletType type,Player* owner)
{
	float playerDamage;
	float terrainDamage;
	float areaRadius;
	float scalarKnockback;

	switch(type)
	{
	case BulletType::rocket:
		playerDamage = rocketPlayerDamage;
		terrainDamage = rocketTerrainDamage;
		areaRadius = rocketDamageRadius;
		scalarKnockback = rocketScalarKnockback;
		break;
	}

	Bullet* bullet = new Bullet{ position,angle,playerDamage,terrainDamage,rocketScalarKnockback,areaRadius,owner,players,terrain,type,bulletProgram };
	bullets.push_back(bullet);
}

void BulletManager::deleteBullet(int index)
{
	delete bullets.at(index);	//Free the memory of the bullet
	bullets.erase(bullets.begin() + index);	//Remove the bullet from the array
}

void BulletManager::deleteBullet(Bullet* bullet)
{
	for(int i=0;i<bullets.size();i++)	//Find the bullet
	{
		if(bullets.at(i)==bullet)
		{
			deleteBullet(i);
			return;
		}
	}
	std::cout << "Bullet not found!" << '\n';
}



void BulletManager::update(float deltaTime)
{
	for(int i=0;i<bullets.size();i++)
	{
		Bullet* currentBullet = bullets.at(i);
		currentBullet->update(deltaTime);
		if(currentBullet->getDelete())
		{
			deleteBullet(i);
		}
	}
	
}

void BulletManager::render()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->render();
	}
}
