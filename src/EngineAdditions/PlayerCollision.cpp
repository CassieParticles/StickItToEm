#include "PlayerCollision.h"

#include <Engine/Collision.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "../Game/TerrainManager.h"
#include "../Game/Player.h"

namespace Collision
{
	bool checkRectTerrain(rect* r, TerrainManager* terrainManager, line** linesColliding, int* collisions)
	{
		std::vector<line> collidingLines{};

		//Get the lines to be checked
		int lineCount;
		line* lines = terrainManager->getLines(r->blCorner, glm::ivec2(ceil(r->size.x + 1), ceil(r->size.y + 1)), &lineCount);

		bool collide{};

		for (int i = 0; i < lineCount; i++)	//Iterate through each line
		{
			line currentLine = lines[i];

			if (currentLine.A == glm::vec2{0, 0}&& currentLine.B == glm::vec2{0, 0}) { continue; }//Skip lines that are empty

			line l;
			if (checkLineRect(r, &currentLine, &l))	//Check if the line is collding with the rect
			{
				//If pointer isn't null, then data is wanted back, put colliding line into std::vector
				if (linesColliding != nullptr) { collidingLines.push_back(l); }
				collide = true;
			}
		}
		delete[] lines;	//Free memory

		if (linesColliding != nullptr)
		{
			line* collidingLineArr = new line[collidingLines.size()];	//Copy colliding line array into memory on heap, so it's preserved out of scope
			std::copy(collidingLines.data(), collidingLines.data() + collidingLines.size(), collidingLineArr);

			*linesColliding = collidingLineArr;
			*collisions = collidingLines.size();
		}

		return collide;
	}

	void resolvePlayerLine(Player* player, line* l, float deltaTime)
	{
		glm::vec2 lineDelta = l->B - l->A;					//Get the line's direction and normal
		glm::vec2 lineDir = glm::normalize(lineDelta);
		glm::vec2 lineNormal = { lineDir.y,-lineDir.x };

		glm::vec2 forceParr = glm::dot(lineDir, player->sumForce) * lineDir;	//Get the parallel and perpendicular components of force and velocity to the line
		glm::vec2 forcePerp = player->sumForce - forceParr;

		glm::vec2 velocityParr = glm::dot(lineDir, player->velocity) * lineDir;
		glm::vec2 velocityPerp = player->velocity - velocityParr;

		//Add check if player should be grounded
		float lineSteepness = abs(glm::dot({ 0,1 }, lineDir));

		glm::vec2 playerCentre = player->position + player->playerSize / 2.f;	//Get if the line is above or below the player
		glm::vec2 dirToLine = glm::normalize(l->A - playerCentre);

		float dotToLine = glm::dot(dirToLine, { 0,1 });

		if (lineSteepness < player->steepnessMax)
		{
			if (dotToLine < 0)
			{
				player->grounded = true;
				if (abs(lineDir.y) > player->forward.y)
				{
					player->forward = lineDir;
					if (player->forward.x < 0) { player->forward *= -1; }
				}
			}
		}

		//Change velocity to stop player moving into the terrain
		if (glm::dot(lineNormal, velocityPerp) < 0)
		{
			player->velocity -= velocityPerp;
		}

		if (glm::dot(lineNormal, forcePerp) < 0)
		{
			player->addForce(-forcePerp);

			glm::vec2 maxFrictionForce = glm::vec2{ forcePerp.y,-forcePerp.x } *player->frictionCoeffecient;
			if (glm::dot(maxFrictionForce, velocityParr) < 0) { maxFrictionForce *= -1; }

			glm::vec2 minForce = -velocityParr * player->mass / deltaTime;

			if (abs(minForce.x) < abs(maxFrictionForce.x))
			{
				player->addForce(minForce);
			}
			else
			{
				player->addForce(-maxFrictionForce);
			}
		}
	}
}