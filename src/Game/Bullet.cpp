#include "Bullet.h"

#include <GLAD/glad.h>

#include <Engine/Animation.h>
#include <Engine/Program.h>
#include <Engine/MathsFunctions.h>

#include "Player.h"
#include "TerrainManager.h"
#include "../EngineAdditions/PlayerCollision.h"

Bullet::Bullet(glm::vec2 position, float angle, Player* playerFired, std::vector<Player*>* players, TerrainManager* terrain, BulletType type, Program* bulletProgram, Gunsmoke* gunSmokeManager) :position{ position }, angle{ angle }, playerFired{ playerFired }, players{ players }, type { type }, bulletProgram{ bulletProgram }, terrain{ terrain },gunSmokeManager{gunSmokeManager}
{
	switch(type)
	{
	case BulletType::rocket:
		velocity = glm::vec2{ cos(angle),sin(angle) } * rocketBulletSpeed;	//Set  bullet details
		playerDamage = rocketPlayerDamage;
		terrainDamage = rocketTerrainDamage;
		forceScalar = rocketScalarKnockback;
		areaRadius = rocketDamageRadius;

		bulletAnim = new Animation{ "assets/Weapons/Rocket.png",2,bulletProgram,true };
		size = bulletAnim->getFrameSize()/20.f;
		break;
	case BulletType::shotgunPellet:
		velocity = glm::vec2{ cos(angle),sin(angle) } * shotgunBulletSpeed;	//Set  bullet details
		playerDamage = shotgunPlayerDamage;
		terrainDamage = shotgunTerrainDamage;
		forceScalar = shotgunScalarKnockback;
		areaRadius = shotgunDamageRadius;

		bulletAnim = new Animation{ "assets/Weapons/Bullet.png",1,bulletProgram,true };
		size = bulletAnim->getFrameSize() / 20.f;

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

	line l{position-(velocity * deltaTime), position};	//Line of the bullets movement

	rect r = getCollisionRect();

	if(Collision::checkRectTerrain(&r,terrain,nullptr,nullptr) || Collision::checkLineTerrain(&l, terrain, nullptr, nullptr))	//If bullet collides with the terrain
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
		terrain->modifyTerrainCircle(getCentre() - glm::vec2{1, 1}, areaRadius, -terrainDamage);
		gunSmokeManager->drawSmoke(getCentre(), areaRadius * 2, glm::vec3(0, 0, 0));
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
					if (players->at(i) == playerFired) { continue; }
					Player* currentPlayer = players->at(i);
					glm::vec2 playerCentre = currentPlayer->getPosition() + currentPlayer->getSize() / 2.f;
					float radSquare = areaRadius * areaRadius;
					glm::vec2 dPos = playerCentre - position;
					float dSqr = glm::dot(dPos, dPos);

					if (true)
					{
						glm::vec2 dir = dPos / sqrt(dSqr);

						currentPlayer->takeDamage(playerDamage);
						currentPlayer->takeKnockback(dir * forceScalar);
					}
				}
				terrain->modifyTerrainCircle(getCentre()-glm::vec2{0,1}, areaRadius, -terrainDamage);
				gunSmokeManager->drawSmoke(getCentre(), areaRadius * 2, glm::vec3(0, 0, 0));
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
	r.angle = angle;
	return r;
}

rect Bullet::getBoundingRect()	
{
	rect r{};

	glm::vec2 offsets[4] =	//Offsets for each of the 4 corners
	{
		{0,0},				//bl
		{size.x,0},			//br
		{0,size.y},			//tl
		{size.x,size.y},	//tr
	};

	const float cosAngle = cos(angle);	//Get cosine and sin of angle
	const float sinAngle = sin(angle);

	glm::mat2x2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};	//Rotation matrix

	offsets[1] = rotMatrix * offsets[1];	//Rotate offsets around bl corner (0,0 wouldn't move, so no need to rotate it)
	offsets[2] = rotMatrix * offsets[2];
	offsets[3] = rotMatrix * offsets[3];

	glm::vec2 bl = {};
	glm::vec2 tr = {};

	for (int i = 1; i < 4; ++i)
	{
		bl.x = std::min(bl.x, offsets[i].x);
		bl.y = std::min(bl.y, offsets[i].y);

		tr.x = std::max(tr.x, offsets[i].x);
		tr.y = std::max(tr.y, offsets[i].y);
	}

	r.blCorner = bl;
	r.size = tr - bl;
	r.angle = 0;

	return r;
}

glm::vec2 Bullet::getCentre()
{
	const float cosAngle = cos(angle);	//Get cosine and sin of angle
	const float sinAngle = sin(angle);

	glm::mat2x2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};	//Rotation matrix

	glm::vec2 direction = size / 2.f;	//Get the direction from the bottom left to the centre
	direction = rotMatrix * direction;	//Pre-multiply to get direction rotated

	return position + direction;
}
