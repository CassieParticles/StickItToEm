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
		addForce({ -5*getMass(),0});
	}
	if (input->getKeyDown(GLFW_KEY_D))
	{
		addForce({ 5 * getMass(),0 });
	}
	if (input->getKeyDown(GLFW_KEY_W))
	{
		addForce({ 0,5 * getMass() });
	}
	if (input->getKeyDown(GLFW_KEY_S))
	{
		addForce({ 0, -5 * getMass() });
	}
	if (input->getKeyDown(GLFW_KEY_BACKSPACE))
	{
		velocity = {};
	}
}

void Player::update(float deltaTime)
{
	//Get the lines the player may be intersecting with
	int lineCount{};
	line* lines = terrainManager->getLines(position, glm::ivec2{ceil(playerSize.x+1), ceil(playerSize.y+1)}, &lineCount);

	rect playerCollisionRect = getCollisionRect();

	static int count{};

	for (int i = 0; i < lineCount; i++)
	{
		line currentLine = lines[i];
		if (currentLine.A != glm::vec2{0, 0}||currentLine.B != glm::vec2{0, 0})
		{
			
			line collidingLine;
			if (Collision::checkLineRect(&playerCollisionRect, &currentLine, &collidingLine))
			{
				std::cout << "Colliding"<<count++<<'\n';
			}
		}
	}

	//Update velocity and position
	//F=ma, a=F/m
	velocity += deltaTime * sumForce / mass;
	sumForce = {};
	position += deltaTime * velocity;

	delete[] lines;	//Delete the lines to prevent memory leak
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
