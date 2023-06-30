#pragma once
#include <glm.hpp>

struct line;

namespace Collision
{
	bool checkLineLine(line* lineA, line* lineB, glm::vec2* point);
}