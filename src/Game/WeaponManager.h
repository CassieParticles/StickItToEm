#pragma once

#include <vector>

#include "Weapon.h"

class TerrainManager;
class Player;
class Program;
class BulletManager;

class WeaponManager
{
public:
	WeaponManager(TerrainManager* terrainManager);
	~WeaponManager();

	void addPlayer(Player* player);

	void createWeapon(WeaponType type, glm::vec2 position);
	void deleteWeapon(int index);
	void deleteWeapon(Weapon* weapon);

	void update(float deltaTime);
	void render();	//Render all the weapons
private:
	BulletManager* bulletManager;

	TerrainManager* terrain;

	std::vector<Player*> players;

	std::vector<Weapon*> weapons;

	Program* weaponProgram;
	
	bool checkPlayersWeapon(Weapon* weapon,Player** player);
};