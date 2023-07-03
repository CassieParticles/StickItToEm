#pragma once
#include <glm.hpp>

struct line;
struct rect;

class TerrainManager;

//Collisions return a boolean of if a collision occured, pointers used to return any other needed info
namespace Collision
{
	bool checkLineLine(line* lineA, line* lineB, glm::vec2* collidingPoint);	
	bool checkLineRect(rect* r, line* l, line* collidingLine);
	bool checkRectTerrain(rect* r, TerrainManager* terrainManager, line** linesColliding, int* collisions);

	bool checkPointRect(rect* r, glm::vec2* point);
}