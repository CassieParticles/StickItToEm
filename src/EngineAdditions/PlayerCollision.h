#pragma once

struct line;
struct rect;

class TerrainManager;
class Player;

namespace Collision
{
	bool checkRectTerrain(rect* r, TerrainManager* terrainManager, line** linesColliding, int* collisions);
	bool checkLineTerrain(line* r, TerrainManager* terrainManager, line** linesColliding, int* collisions);

	void resolvePlayerLine(Player* player, line* l, float deltaTime);
}