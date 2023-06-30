#include "Collision.h"
#include "../Game/TerrainManager.h"

namespace Collision
{
	bool checkLineLine(line* lineA, line* lineB, glm::vec2* point)
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

		if (point != nullptr && collide)
		{
			*point = lineA->A + dA * t1;
		}

		return collide;
	}
}