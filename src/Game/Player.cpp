#include "Player.h"

#include <glad/glad.h>

#include "../Engine/Program.h"

Player::Player(glm::ivec2 gridSize, glm::vec2 position, float mass) :position{position},mass{mass}
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
	if()
}

void Player::update(float deltaTime)
{


	//Update velocity and position
	//F=ma, a=F/m
	velocity += deltaTime * sumForce / mass;
	sumForce = {};
	position += deltaTime * velocity;
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

void Player::addForce(glm::vec2 force)
{
	sumForce += force;
}
