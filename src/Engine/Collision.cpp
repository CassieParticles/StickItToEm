#include "Collision.h"

#include "MathsFunctions.h"

#include "../Game/TerrainManager.h"
#include "../Game/Player.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace Collision
{
	bool checkLineLine(line* lineA, line* lineB, glm::vec2* collidingPoint)
	{
		glm::vec2 dA = lineA->B - lineA->A;	//Get the vector from point A to B on each line
		glm::vec2 dB = lineB->B - lineB->A;

		float denom = dB.y * dA.x - dB.x * dA.y;
		if (denom == 0)	//If denom is 0, then the lines are parallel
		{
			return false;
		}

		float invDenom = 1.f / denom;

		float t1 = (dB.x * (lineA->A.y - lineB->A.y) - dB.y * (lineA->A.x - lineB->A.x)) * invDenom;
		float t2 = (dA.x * (lineA->A.y - lineB->A.y) - dA.y * (lineA->A.x - lineB->A.x)) * invDenom;

		bool collide = t1 >= 0 && t1 <= 1 && t2>=0 && t2 <= 1;

		if (collidingPoint != nullptr && collide)
		{
			*collidingPoint = lineA->A + dA * t1;
		}

		return collide;
	}

	bool checkLineRect(rect* r, line* l, line* collidingLine)
	{
		float cosAngle = cos(r->angle);	//Get cosine and sin of rotated angle
		float sinAngle = sin(r->angle);
		glm::mat2x2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};	//Rotate the corners

		glm::vec2 tl = r->tlCorner;
		glm::vec2 tr = r->tlCorner + rotMatrix * glm::vec2{r->size.x, 0};
		glm::vec2 bl = r->tlCorner + rotMatrix * glm::vec2{0, r->size.y};
		glm::vec2 br = r->tlCorner + rotMatrix * r->size;
		
		line edges[4] = {  
			{tl,tr},
			{tr,br},
			{br,bl},
			{bl,tl}
		};

		bool collide{};

		bool gotFirstCollisionPoint{};	
		bool gotSecondCollisionPoint{};

		if (checkPointRect(r, &l->A))	//Check to see if either end of the line is in the rect
		{
			gotFirstCollisionPoint = true;
			collide = true;
			if (collidingLine != nullptr) { collidingLine->A = l->A; }
		}

		if (checkPointRect(r, &l->B))
		{
			gotSecondCollisionPoint = true;
			collide = true;
			if (collidingLine != nullptr) { collidingLine->B = l->B; }
		}

		for (int i = 0; i < 4; i++)	//Collision is checked by creating 4 lines to represent the sides of the rect, and checking each of these
		{
			glm::vec2* point{nullptr};		
			if (collidingLine == nullptr)
			{
				point = nullptr;
			}
			else if (!gotFirstCollisionPoint)	//If the first point isn't gotten yet
			{
				point = &collidingLine->A;
			}
			else if (!gotSecondCollisionPoint)	//If the first point is gotten, but the second one isn't
			{
				point = &collidingLine->B;
			}
			else								//If both points are gotten
			{
				break;
			}

			bool currentLineCollide = checkLineLine(edges + i, l, point);

			//Boolean logic to if the first and second vertices are gotten
			gotSecondCollisionPoint = currentLineCollide && gotFirstCollisionPoint || gotSecondCollisionPoint;
			gotFirstCollisionPoint = currentLineCollide || gotFirstCollisionPoint;
			

			collide = collide || currentLineCollide;
		}

		if(collidingLine==nullptr)
		{
			return collide;
		}

		if (MathsFunctions::getDistSqr(collidingLine->A, l->A) > MathsFunctions::getDistSqr(collidingLine->B, l->A))
		{
			//Line is wrong way round, swap it
			glm::vec2 temp = collidingLine->A;
			collidingLine->A = collidingLine->B;
			collidingLine->B = temp;
		}

		return collide;
	}

	bool checkRectTerrain(rect* r, TerrainManager* terrainManager, line** linesColliding, int* collisions)
	{
		std::vector<line> collidingLines{};

		//Get the lines to be checked
		int lineCount;
		line* lines = terrainManager->getLines(r->tlCorner, glm::ivec2(ceil(r->size.x + 1), ceil(r->size.y + 1)), &lineCount);

		bool collide{};

		for (int i = 0; i < lineCount; i++)	//Iterate through each line
		{
			line currentLine = lines[i];	

			if (currentLine.A == glm::vec2{0, 0} && currentLine.B == glm::vec2{0, 0}) { continue; }//Skip lines that are empty

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

	bool checkPointRect(rect* r, glm::vec2* point)
	{
		float cosAngle = cos(r->angle);	//Get cosine and sin of rotated angle
		float sinAngle = sin(r->angle);
		glm::mat2x2 rotMatrix{cosAngle, -sinAngle, sinAngle, cosAngle};	//Rotate the corners
		glm::mat2x2 invRotMatrix = glm::inverse(rotMatrix);

		glm::vec2 translatedPoint = *point - r->tlCorner;
		glm::vec2 rotatedPoint = invRotMatrix * translatedPoint;
		
		return rotatedPoint.x > 0 && rotatedPoint.x < r->size.x && rotatedPoint.y > 0 && rotatedPoint.y < r->size.y;
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

			glm::vec2 maxFrictionForce = glm::vec2{ forcePerp.y,-forcePerp.x } * player->frictionCoeffecient;
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