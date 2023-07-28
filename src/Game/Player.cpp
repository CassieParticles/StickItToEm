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

Player::Player(Input* input, glm::ivec2 gridSize, glm::vec2 position, float mass) :input{ input },position { position }, mass{ mass }
{
	constexpr glm::vec2 vertexPos[6]{
		glm::vec2{0,0},
		glm::vec2{1,1},
		glm::vec2{1,0},
		glm::vec2{0,0},
		glm::vec2{0,1},
		glm::vec2{1,1}
	};

	constexpr int cornerIndex[6]{	//Which 
		2,
		1,
		3,
		2,
		0,
		1
	};

	glGenVertexArrays(1, &vaoID);	//Generate and bind vertex array
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertVBO);		//Generate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);	//Bind and fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &cornerIndexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cornerIndexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cornerIndex), cornerIndex, GL_STATIC_DRAW);
	glVertexAttribIPointer(1, 1, GL_INT, sizeof(int), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	

	playerProgram = new Program{ "src/Shaders/Player/vertex.glsl","src/Shaders/Player/fragment.glsl" };	//Create program to render player

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
	glDeleteBuffers(1, &vertVBO);
	glDeleteBuffers(1, &cornerIndexVBO);
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
	if (input->getKeyDown(GLFW_KEY_A))
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
	if (input->getKeyDown(GLFW_KEY_D))
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
	if (input->getKeyPressed(GLFW_KEY_W))
	{
		if (grounded)
		{
			addForce(glm::vec2{ 0,1 } * playerJumpForce * mass / deltaTime);
			grounded = false;
			changeAnimation(jump);
		}
	}

	if (input->getKeyDown(GLFW_KEY_BACKSPACE))
	{
		velocity = {};
		position = {30,20};
	}

	if (input->getKeyPressed(GLFW_KEY_F))
	{
		currentAnimation->flipAnim();
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

	currentAnimation->setCurrentFrame();

	glBindVertexArray(vaoID);	//Use player vertex array
	glDrawArrays(GL_TRIANGLES,0,6);

	glBindVertexArray(0);
}

rect Player::getCollisionRect()
{
	rect r{};
	r.tlCorner = position;
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
