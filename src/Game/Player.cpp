#include "Player.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include <Engine/Program.h>
#include <Engine/Input.h>
#include <Engine/Collision.h>
#include <Engine/Animation.h>

#include "../EngineAdditions/PlayerCollision.h"

#include "TerrainManager.h"
#include "Weapon.h"

Player::Player(Input* input, glm::vec2 position, float mass,glm::vec3 colour) :input{ input },position { position }, mass{ mass },colour{colour},weapon{nullptr}
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
	

	playerProgram = new Program{ "src/Shaders/Player/player.vert","src/Shaders/Player/player.frag", Program::filePath};	//Create program to render player

	playerProgram->setUniformBufferBlockBinding("terrainData", 1);

	playerIdleAnim = new Animation("assets/playerAnimation/FistStanding.png", 1, playerProgram, true);	//Initialise all the player animations
	playerWalkAnim = new Animation("assets/playerAnimation/FistWalking.png", 7, playerProgram,true);
	playerJumpAnim = new Animation("assets/playerAnimation/FistJump.png", 7, playerProgram, false);
	playerFallAnim = new Animation("assets/playerAnimation/FistFalling.png", 1, playerProgram, true);

	playerIdleAnim->setFrameTime(0.1f);
	playerWalkAnim->setFrameTime(0.1f);
	playerJumpAnim->setFrameTime(0.1f);
	playerFallAnim->setFrameTime(0.1f);

	currentAnimation = playerIdleAnim;
}

Player::~Player()
{
	glDeleteBuffers(3, vertexBuffers);
	glDeleteVertexArrays(1, &vaoID);
	vaoID = 0;
	

	delete playerProgram;

	delete playerIdleAnim;
	delete playerWalkAnim;
	delete playerJumpAnim;
	delete playerFallAnim;
}

void Player::handleInput(float deltaTime)
{
	if (input->getKeyDown(con.moveLeft))
	{
		if (grounded)
		{
			if (glm::dot(forward, -velocity) < playerRunMaxSpeed)
			{
				addForce(-forward * playerRunForce * mass);
			}
		}
		else
		{
			if (glm::dot(forward, -velocity) < playerAirMaxSpeed)
			{
				addForce(-forward * playerAirForce * mass);
			}
		}
	}
	if (input->getKeyDown(con.moveRight))
	{
		if (grounded)
		{
			if (glm::dot(forward, velocity) < playerRunMaxSpeed)
			{
				addForce(forward * playerRunForce * mass);
			}
		}
		else
		{
			if (glm::dot(forward, velocity) < playerAirMaxSpeed)
			{
				addForce(forward * playerAirForce * mass);
			}
		}
	}
	if (input->getKeyPressed(con.jump))
	{
		if (grounded)
		{
			addForce(glm::vec2{ 0,1 } * playerJumpForce * mass / deltaTime);
			grounded = false;
			changeAnimation(jump);
		}
	}
	if(input->getKeyDown(con.aimUp))
	{
		aimAngle += playerAimSpeed * deltaTime;
	}
	if(input->getKeyDown(con.aimDown))
	{
		aimAngle -= playerAimSpeed * deltaTime;
	}
	if(input->getKeyPressed(con.shoot))
	{
		if(weapon!=nullptr)
		{
			weapon->fireWeapon();
		}
	}

	if (input->getKeyDown(GLFW_KEY_BACKSPACE))
	{
		velocity = {};
		position = {30,20};
	}

}

void Player::collisionResolution(float deltaTime)
{
	rect playerCollisionRect = getCollisionRect();

	line* collidingLines;	//Values to be returned from collision calculation
	int collisionCount;

	bool collision;

	//Get if a collision occurs and if so, get the lines the player collides with
	collision = Collision::checkRectTerrain(&playerCollisionRect, terrainManager, &collidingLines, &collisionCount);

	/*Since forces aren't resolved till after all collisions are accounted for, some times collisions need this to 
	offset collisions with multiple different lines in the same time*/
	
	float lineCountOffset = 1.f / collisionCount;	
	grounded = false;
	forward = { 1,0 };

	for (int i = 0; i < collisionCount; i++)
	{
		Collision::resolvePlayerLine(this, collidingLines+i, deltaTime);	//Resolve the collision between the player and that line

		for (int j = 0; j < i; j++)	//Resolve all previous lines
		{
			Collision::resolvePlayerLine(this, collidingLines + j, deltaTime);
		}

	}



	delete[] collidingLines;
}

void Player::update(float deltaTime)
{
	currentAnimation->update(deltaTime);


	//Update velocity and position
	//F=ma, a=F/m
	velocity += deltaTime * sumForce / mass;
	sumForce = {};
	position += deltaTime * velocity;

	//If player jump animation has finished, switch to fall, where it is judged whether or not it should be changed
	if (currentAnimation == playerJumpAnim && playerJumpAnim->getFinished())
	{
		changeAnimation(fall);
	}

	else if (currentAnimation == playerFallAnim && grounded)
	{
		changeAnimation(idle);
	}

	if (grounded && abs(velocity.x) > 0.05f && currentAnimation != playerWalkAnim && currentAnimation != playerJumpAnim)
	{
		changeAnimation(walk);
	}
	
	if (grounded && abs(velocity.x) < 0.05f && currentAnimation != playerIdleAnim && currentAnimation != playerJumpAnim)
	{
		changeAnimation(idle);
	}

	if (!grounded && currentAnimation != playerJumpAnim && currentAnimation !=playerFallAnim)
	{
		changeAnimation(fall);
	}

	if (velocity.x < -0.05f)
	{
		flipped = true;
	}
	else if (velocity.x > 0.05f)
	{
		flipped = false;
	}

	currentAnimation->setFlipped(flipped);

	addForce(gravForce*mass);	//Add gravity for next frame, applied here so it's before collision resolution is done
}

void Player::render()
{
	playerProgram->use();	//Use shader, and set uniforms
	playerProgram->setVec2("position",position);
	playerProgram->setVec2("size", playerSize);
	playerProgram->setVec3("colour", colour);

	currentAnimation->setCurrentFrame();

	glBindVertexArray(vaoID);	//Use player vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffers[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

rect Player::getCollisionRect()
{
	rect r{};
	r.blCorner = position;
	r.size = playerSize;
	r.angle = 0;
	return r;
}

void Player::addForce(glm::vec2 force)
{
	sumForce += force;
}

void Player::changeAnimation(playerAnimations newAnimation)
{
	currentAnimation->reset();
	
	switch (newAnimation)
	{
	case idle:
		currentAnimation = playerIdleAnim;
		break;
	case walk:
		currentAnimation = playerWalkAnim;
		break;
	case jump:
		currentAnimation = playerJumpAnim;
		break;
	case fall:
		currentAnimation = playerFallAnim;
		break;
	}
}

void Player::setWeapon(Weapon* weapon)
{
	this->weapon = weapon;
}

float Player::getAimAngle()
{
	//If flipped is true, returns 3.14 - aimAngle
	//if flipped is false, returns aimAngle
	return 3.14159 * flipped + (1 - 2 * flipped) * aimAngle;	
}
