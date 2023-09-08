#include "WeaponManager.h"

#include <Engine/Collision.h>
#include <Engine/Program.h>

#include "Weapon.h"
#include "Player.h"

WeaponManager::WeaponManager(TerrainManager* terrainManager):terrain{terrainManager}
{
	weaponProgram = new Program{ "src/Shaders/weapon/weapons.vert","src/Shaders/weapon/weapons.frag",Program::filePath };
}

WeaponManager::~WeaponManager()
{
	for(int i=0;i<weapons.size();i++)
	{
		delete weapons.at(i);
	}
	delete weaponProgram;
}


void WeaponManager::addPlayer(Player* player)
{
	players.push_back(player);
}

void WeaponManager::createWeapon(WeaponType type, glm::vec2 position)
{
	Weapon* w = new Weapon(position, type, terrain,weaponProgram);
	weapons.push_back(w);
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
				}
			}
		}

		
		weapons.at(i)->update(deltaTime);
	}
}

void WeaponManager::render()
{
	for(int i=0;i<weapons.size();i++)
	{
		weapons.at(i)->render();
	}
}
