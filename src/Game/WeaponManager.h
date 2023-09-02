#pragma once

#include <vector>

#include "Weapon.h"

class TerrainManager;
class Player;

class WeaponManager
{
public:
	WeaponManager(TerrainManager* terrainManager);
	~WeaponManager();

	void addPlayer(Player* player);

	void createWeapon(WeaponType type, glm::vec2 position);

	void update(float deltaTime);
	void render();	//Render all the weapons
private:
	TerrainManager* terrain;

	std::vector<Player*> players;

	std::vector<Weapon*> weapons;
	
	bool checkPlayersWeapon(Weapon* weapon,Player** player);
};