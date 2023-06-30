#pragma once
#include <glm.hpp>

struct line;
struct rect;

namespace Collision
{
	bool checkLineLine(line* lineA, line* lineB, glm::vec2* collidingPoint);
	bool checkLineRect(rect* r, line* l, line* collidingLine);

}