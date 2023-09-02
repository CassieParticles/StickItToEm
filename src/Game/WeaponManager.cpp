#include "WeaponManager.h"

#include "Engine/Collision.h"

#include "Weapon.h"
#include "Player.h"

WeaponManager::WeaponManager(TerrainManager* terrainManager):terrain{terrainManager}
{
	
}

WeaponManager::~WeaponManager()
{
	for(int i=0;i<weapons.size();i++)
	{
		delete weapons.at(i);
	}
}


void WeaponManager::addPlayer(Player* player)
{
	players.push_back(player);
}

void WeaponManager::createWeapon(WeaponType type, glm::vec2 position)
{
	Weapon* w = new Weapon(position, type, terrain);
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
		if(checkPlayersWeapon(weapons.at(i), &collidingPlayer))
		{
			weapons.at(i)->setWielder(collidingPlayer);
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
