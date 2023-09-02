#pragma once
#include <glm.hpp>

class Player;
class Animation;
class Program;

class Weapon
{
public:
	enum WeaponType
	{
		rocketLauncher
	};

	Weapon(glm::vec2 position,  WeaponType type);
	~Weapon();

	void update(float deltaTime);
	void render();

protected:
	glm::vec2 position;
	glm::vec2 size;

	WeaponType type;

	Player* wielder=nullptr;

	unsigned int vaoID;	//Vertex data
	unsigned int vertexBuffers[3];

	Program* weaponProgram;

	unsigned int textureID;
	Animation* idleAnimation;
};