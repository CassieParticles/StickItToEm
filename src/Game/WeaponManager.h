#pragma once

#include <vector>
#include <random>

#include "Weapon.h"

class TerrainManager;
class Player;
class Program;
class BulletManager;
class Gunsmoke;

class WeaponManager
{
public:
	WeaponManager(TerrainManager* terrainManager, Gunsmoke* gunSmokeManager, std::mt19937* rand);
	~WeaponManager();

	void addPlayer(Player* player);

	void createWeapon(WeaponType type, glm::vec2 position);
	void deleteWeapon(int index);
	void deleteWeapon(Weapon* weapon);

	void spawnRandomWeapon(glm::vec2 position, std::vector<WeaponType> typesPermitted);

	void update(float deltaTime);
	void render();	//Render all the weapons
private:
	BulletManager* bulletManager;

	TerrainManager* terrain;

	std::vector<Player*> players;

	std::vector<Weapon*> weapons;

	std::mt19937* rand;

	Program* weaponProgram;
	
	bool checkPlayersWeapon(Weapon* weapon,Player** player);
};