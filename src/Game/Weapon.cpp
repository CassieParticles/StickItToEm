#include "Weapon.h"

#include <Engine/TextureManager.h>

#include <glad/glad.h>

#include "Engine/Program.h"
#include "Engine/Animation.h"

#include "Player.h"

Weapon::Weapon(glm::vec2 position, WeaponType type) :position{ position },type{type}
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

	weaponProgram = new Program("src/Shaders/weapon/weapons.vert", "src/Shaders/weapon/weapons.frag", Program::filePath);

	texture tex;
	switch (type)
	{
	case rocketLauncher:
		tex=TextureManager::getTexturePtr("assets/Weapons/RocketLauncher.png");	//Get the texture for the weapon

		textureID = tex.textureID;	//Extract the textureID and the size
		size = glm::vec2{ tex.width / 20.f,tex.height / 20.f };

		idleAnimation = new Animation(tex, 1, weaponProgram, true);

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
	
}

void Weapon::update(float deltaTime)
{
}

void Weapon::render()
{
	glBindVertexArray(vaoID);
	weaponProgram->use();

	weaponProgram->setVec2("position", position);
	weaponProgram->setVec2("size", size);

	weaponProgram->setInt("spriteSheet", 0);

	idleAnimation->setCurrentFrame();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
