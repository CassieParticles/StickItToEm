#include "Weapon.h"

#include <Engine/TextureManager.h>

#include <glad/glad.h>

#include <Engine/Program.h>
#include <Engine/Animation.h>

#include "Player.h"
#include "TerrainManager.h"
#include "BulletManager.h"

Weapon::Weapon(glm::vec2 position, WeaponType type, TerrainManager* terrainManager,Program* weaponProgram,BulletManager* bulletManager) :position{ position },type{type},terrain(terrainManager),angle{0.f},weaponProgram{weaponProgram},bulletManager{bulletManager}
{
	constexpr glm::vec2 vertexPos[4]{
	glm::vec2{0,0},
	glm::vec2{1,1},
	glm::vec2{1,0},
	glm::vec2{0,1},
	};

	constexpr int indices[6]
	{
		0,1,2,0,3,1
	};

	constexpr int cornerIndex[4]{	//Which corner each vertex is
		2,
		1,
		3,
		0,
	};


	texture tex;
	switch (type)
	{
	case WeaponType::rocketLauncher:
		tex=TextureManager::getTexturePtr("assets/Weapons/RocketLauncher.png");	//Get the texture for the weapon

		textureID = tex.textureID;	//Extract the textureID and the size
		size = glm::vec2{ tex.width / 20.f,tex.height / 20.f };

		idleAnimation = new Animation(tex, 1, weaponProgram, true);

		playerHandOffset = { -0.4f,1.5f };

		break;
	}

	glGenVertexArrays(1, &vaoID);	//Generate and bind vertex array
	glBindVertexArray(vaoID);

	glGenBuffers(3, vertexBuffers);		//Generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);	//Bind and fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cornerIndex), cornerIndex, GL_STATIC_DRAW);
	glVertexAttribIPointer(1, 1, GL_INT, sizeof(int), 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	weaponProgram->setUniformBufferBlockBinding("terrainData", 1);
}

Weapon::~Weapon()
{
	if (wielder != nullptr)
	{
		wielder->setWeapon(nullptr);
		wielder = nullptr;
	}

	delete idleAnimation;
}

rect Weapon::getCollisionRect()
{
	rect r{};
	r.blCorner = position;
	r.size = size;
	r.angle = 0;
	return r;
}

void Weapon::update(float deltaTime)
{
	if(wielder==nullptr)
	{
		rect r = getCollisionRect();
		bool colliding = Collision::checkRectTerrain(&r, terrain, nullptr, nullptr);

		if (position.x<0 || position.x>terrain->getArenaSize().x || position.y<0 || position.y>terrain->getArenaSize().y)
		{
			setDelete();
		}

		if (!colliding)
		{
			position += gravityVel * deltaTime;
		}
	}
	else
	{
		position = wielder->getPosition()+playerHandOffset;
		angle = wielder->getAngle();
		idleAnimation->setFlipped(wielder->getFlipped());
	}
	
}

void Weapon::render()
{
	glBindVertexArray(vaoID);
	weaponProgram->use();

	weaponProgram->setVec2("position", position);
	weaponProgram->setVec2("size", size);

	float cosAngle = cos(angle);
	float sinAngle = sin(angle);

	glm::mat2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};


	weaponProgram->setMat2("rotMatrix",rotMatrix);

	weaponProgram->setInt("spriteSheet", 0);

	idleAnimation->setCurrentFrame();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Weapon::fireWeapon()
{
	//If player isn't flipped, angle is normal
	//If player is flipped, angle is 180 degrees, minus the angle
	float fireAngle = wielder->getAimAngle();

	switch(type)
	{
	case WeaponType::rocketLauncher:
		bulletManager->addBullet(position, fireAngle, BulletType::rocket,getWielder());
		break;
	}
}
