#include "WeaponManager.h"

#include <iostream>
#include <Engine/Collision.h>
#include <Engine/Program.h>

#include "BulletManager.h"
#include "Weapon.h"
#include "Player.h"
#include "Gunsmoke.h"

WeaponManager::WeaponManager(TerrainManager* terrainManager,Gunsmoke* gunSmokeManager ):terrain{terrainManager}
{
	weaponProgram = new Program{ "src/Shaders/weapon/weapons.vert","src/Shaders/weapon/weapons.frag",Program::filePath };
	bulletManager = new BulletManager{ &players,terrainManager,gunSmokeManager };
}

WeaponManager::~WeaponManager()
{
	for(int i=0;i<weapons.size();i++)
	{
		delete weapons.at(i);
	}
	delete weaponProgram;

	delete bulletManager;
}


void WeaponManager::addPlayer(Player* player)
{
	players.push_back(player);
}

void WeaponManager::createWeapon(WeaponType type, glm::vec2 position)
{
	Weapon* w = new Weapon(position, type, terrain,weaponProgram,bulletManager);
	weapons.push_back(w);
}

void WeaponManager::deleteWeapon(int index)
{
	if(weapons.at(index)->getWielder())
	{
		weapons.at(index)->getWielder()->setWeapon(nullptr);
	}
	delete weapons.at(index);
	weapons.erase(weapons.begin() + index);
}

void WeaponManager::deleteWeapon(Weapon* weapon)
{
	for(int i=0;i<weapons.size();i++)
	{
		if(weapons.at(i)==weapon)
		{
			deleteWeapon(i);
			return;
		}
	}
	std::cout << "Weapon not found!\n";
}



bool WeaponManager::checkPlayersWeapon(Weapon* weapon, Player** player)
{
	rect weaponRect = weapon->getCollisionRect();
	for (int i = 0; i < players.size(); i++)
	{
		rect playerRect = players.at(i)->getCollisionRect();
		if (Collision::checkRectRect(&weaponRect, &playerRect))
		{
			if (player != nullptr)
			{
				*player = players.at(i);
			}
			return true;
		}
	}
	*player = nullptr;
	return false;
}

void WeaponManager::update(float deltaTime)
{
	for(int i=0;i<weapons.size();i++)
	{
		Player* collidingPlayer;
		if(!weapons.at(i)->getWielder())
		{
			if (checkPlayersWeapon(weapons.at(i), &collidingPlayer))
			{
				if (!collidingPlayer->getWeapon()) 
				{
					weapons.at(i)->setWielder(collidingPlayer);
					collidingPlayer->setWeapon(weapons.at(i));
				}
			}
		}
		weapons.at(i)->update(deltaTime);
		if(weapons.at(i)->getDelete())
		{
			deleteWeapon(i);
		}
	}

	bulletManager->update(deltaTime);
}

void WeaponManager::render()
{
	for(int i=0;i<weapons.size();i++)
	{
		weapons.at(i)->render();
	}
	bulletManager->render();
}
