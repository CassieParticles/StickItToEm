#include "Bullet.h"

#include <GLAD/glad.h>

#include <Engine/Animation.h>
#include <Engine/Program.h>
#include <Engine/MathsFunctions.h>

#include "Player.h"
#include "TerrainManager.h"
#include "../EngineAdditions/PlayerCollision.h"

Bullet::Bullet(glm::vec2 position, float angle, float playerDamage, float terrainDamage, float forceScalar, float areaRadius, Player* playerFired, std::vector<Player*>* players, TerrainManager* terrain, BulletType type, Program* bulletProgram) :position{ position }, angle{ angle }, playerDamage{ playerDamage }, areaRadius{ areaRadius }, playerFired{ playerFired }, players{ players }, terrainDamage{ terrainDamage }, forceScalar{ forceScalar }, type { type }, bulletProgram{ bulletProgram }, terrain{ terrain }
{
	switch(type)
	{
	case BulletType::rocket:
		velocity = glm::vec2{ cos(angle),sin(angle) }*rocketBulletSpeed;
		bulletAnim = new Animation{ "assets/Weapons/Rocket.png",2,bulletProgram,true };
		size = bulletAnim->getFrameSize()/20.f;
		break;
	}

	bulletAnim->setFrameTime(0.1f);

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

	bulletProgram->setUniformBufferBlockBinding("terrainData", 1);
}

Bullet::~Bullet()
{
	delete bulletAnim;
}

void Bullet::update(float deltaTime)
{
	position += velocity * deltaTime;
	bulletAnim->update(deltaTime);

	if(position.x<0||position.x>terrain->getArenaSize().x||position.y<0||position.y>terrain->getArenaSize().y)
	{
		setDelete();
	}

	rect r = getCollisionRect();

	if(Collision::checkRectTerrain(&r,terrain,nullptr,nullptr))	//If bullet collides with the terrain
	{
		for(int i=0;i<players->size();i++)
		{
			Player* currentPlayer = players->at(i);
			glm::vec2 playerCentre = currentPlayer->getPosition() + currentPlayer->getSize() / 2.f;
			float radSquare = areaRadius * areaRadius;
			glm::vec2 dPos = playerCentre - position;
			float dSqr = glm::dot(dPos, dPos);

			if(dSqr<radSquare)
			{
				
				glm::vec2 dir = dPos / sqrt(glm::dot(dPos, dPos));

				float scalar = 1 - std::sqrt(dSqr) / std::sqrt(radSquare);
				currentPlayer->takeDamage(playerDamage * scalar);
				currentPlayer->takeKnockback(dir*forceScalar*scalar);
			}
		}
		terrain->modifyTerrainCircle(position, areaRadius, -terrainDamage);
		setDelete();
	}

	for(int i=0;i<players->size();i++)
	{

		if(players->at(i)!=playerFired)
		{
			rect pr = players->at(i)->getCollisionRect();
			if(Collision::checkRectRect(&r,&pr))
			{
				for (int i = 0; i < players->size(); i++)
				{
					Player* currentPlayer = players->at(i);
					glm::vec2 playerCentre = currentPlayer->getPosition() + currentPlayer->getSize() / 2.f;
					float radSquare = areaRadius * areaRadius;
					glm::vec2 dPos = playerCentre - position;
					float dSqr = glm::dot(dPos, dPos);

					if (dSqr < radSquare)
					{

						glm::vec2 dir = dPos / sqrt(glm::dot(dPos, dPos));

						float scalar = 1 - std::sqrt(dSqr) / std::sqrt(radSquare);
						currentPlayer->takeDamage(playerDamage * scalar);
						currentPlayer->takeKnockback(dir * forceScalar * scalar);
					}
				}
				terrain->modifyTerrainCircle(position, areaRadius, -terrainDamage);
				setDelete();
			}
		}
	}
}

void Bullet::render()
{
	glBindVertexArray(vaoID);
	bulletProgram->use();

	bulletProgram->setVec2("position", position);
	bulletProgram->setVec2("size", size);

	float cosAngle = cos(angle);
	float sinAngle = sin(angle);

	glm::mat2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};

	bulletProgram->setMat2("rotMatrix", rotMatrix);

	bulletProgram->setInt("spriteSheet", 0);

	bulletAnim->setCurrentFrame();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

rect Bullet::getCollisionRect()
{
	rect r{};
	r.blCorner = position;
	r.size = size;
	r.angle = 0;
	return r;
}