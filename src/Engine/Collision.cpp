#include "Collision.h"
#include "../Game/TerrainManager.h"
#include "../Game/Player.h"

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

		bool collide = t1 > 0 && t1 < 1 && t2>0 && t2 < 1;

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

		for (int i = 0; i < 4; i++)
		{
			glm::vec2* point{nullptr};
			if (collidingLine==nullptr) 
			{ 
				point == nullptr; 
			}
			else if (gotFirstCollisionPoint) 
			{ 
				point = &(collidingLine->B); 
			}
			else if (gotSecondCollisionPoint) 
			{ 
				point = nullptr; 
			}
			else 
			{ 
				point = &(collidingLine->A); 
			}

			bool currentLineCollide = checkLineLine(edges + i, l, point);

			gotSecondCollisionPoint = (gotFirstCollisionPoint && currentLineCollide || gotSecondCollisionPoint);
			gotFirstCollisionPoint = currentLineCollide || gotFirstCollisionPoint;

			collide = collide || currentLineCollide;
		}

		return collide;
	}
	bool checkRectTerrain(rect* r, TerrainManager* terrainManager, line* linesColliding, int* collisions)
	{
		int lineCount;
		line* lines = terrainManager->getLines(r->tlCorner, glm::ivec2(ceil(r->size.x + 1), ceil(r->size.y + 1)), &lineCount);

		for (int i = 0; i < lineCount; i++)
		{
			line currentLine = lines[i];
			if (currentLine.A != glm::vec2{0, 0} || currentLine.B != glm::vec2{0, 0})
			{
				if (checkLineRect(r, &currentLine, nullptr))
				{
					delete[] lines;
					return true;
				}
			}
		}

		delete[] lines;
		return false;
	}
}