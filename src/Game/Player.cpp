#include "Player.h"

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "../Engine/Program.h"
#include "../Engine/Input.h"
#include "../Engine/Collision.h"

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

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertVBO);

	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	

	playerProgram = new Program{ "src/Shaders/Player/vertex.glsl","src/Shaders/Player/fragment.glsl" };
}

Player::~Player()
{
	glDeleteBuffers(1, &vertVBO);
	glDeleteVertexArrays(1, &vaoID);
	vaoID = 0;
	

	delete playerProgram;
}

void Player::handleInput(float deltaTime)
{
	if (input->getKeyDown(GLFW_KEY_A))
	{
		addForce({ -15*getMass(),0});
	}
	if (input->getKeyDown(GLFW_KEY_D))
	{
		addForce({ 15 * getMass(),0 });
	}
	if (input->getKeyDown(GLFW_KEY_W))
	{
		addForce({ 0,15 * getMass() });
	}
	if (input->getKeyDown(GLFW_KEY_S))
	{
		addForce({ 0, -15 * getMass() });
	}
	if (input->getKeyDown(GLFW_KEY_BACKSPACE))
	{
		velocity = {};
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

	for (int i = 0; i < collisionCount; i++)
	{
		line currentLine = collidingLines[i];	//Get information on the current line being resolved
		glm::vec2 linDir = currentLine.B - currentLine.A;
		glm::vec2 linDirNorm = glm::normalize(linDir);
		glm::vec2 linNormal = { linDirNorm.y,-linDirNorm.x };	//Points out of the terrain

		//Get the force in the components parallel and perpendicular to the line being resolved
		glm::vec2 forceParr = glm::dot(linDirNorm, sumForce) * linDirNorm;	
		glm::vec2 forcePerp = sumForce - forceParr;

		//Get the velocity in the components parallel and perpendicular to the line being resolved
		glm::vec2 velParr = glm::dot(linDirNorm, velocity) * linDirNorm;
		glm::vec2 velPerp = velocity - velParr;

		//Impulse from collision
		if (glm::dot(linNormal, velPerp) < 0)	//If dot product is less then 0, then velocity must be going into the terrain
		{
			addForce(-velPerp * mass * lineCountOffset / deltaTime);
		}

		//Normal contact
		if (glm::dot(linNormal, forcePerp) < 0)	//Same as velocity
		{
			addForce(-forcePerp);

			//Friction applied based on normal contact force
			glm::vec2 frictionForce = glm::vec2{ forcePerp.y,-forcePerp.x } * frictionCoeffecient;	//Maximum friction applied
			if (glm::dot(frictionForce, velParr) < 0) { frictionForce *= -1; }	//Invert friction force if it's the wrong direction

			glm::vec2 minForce = -velParr * mass / deltaTime;	//The minimum force needed to completely stop the player

			if (abs(minForce.x) < abs(frictionForce.x))	//If the minimum force needed is smaller then the maxmimum, apply min
			{
				addForce(minForce);	//Should stop player movement
			}
			else		//Else, apply the maximum
			{
				addForce(-frictionForce);	//Shouldn't stop the player movement
			}
		}
	}
	delete[] collidingLines;
}

void Player::update(float deltaTime)
{
	//Get the lines the player may be intersecting with



	//Update velocity and position
	//F=ma, a=F/m
	velocity += deltaTime * sumForce / mass;
	sumForce = {};
	position += deltaTime * velocity;


	addForce(gravForce*mass);	//Add gravity for next frame, applied here so it's before collision resolution is done
}

void Player::render()
{
	playerProgram->use();	//Use shader, and set uniforms
	playerProgram->setVec2("position",position);
	playerProgram->setVec2("size", playerSize);

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
