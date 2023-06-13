#include <glm.hpp>



struct line
{
	glm::vec2 A;
	glm::vec2 B;
};

struct triangle
{
	glm::vec2 A;
	glm::vec2 B;
	glm::vec2 C;
};

class TerrainManager
{
public:
	TerrainManager(glm::ivec2 arenaSize);	//Take in the size of the arena
	~TerrainManager();
protected:
	glm::ivec2 arenaSize;

	float* scalarData;
	int scalarDataSize;

	line* lineArray;	//2 lines max per square in arena

	unsigned int triangleVAO;	//Buffers for triangle mesh
	unsigned int triangleVBO;

	unsigned int lineVAO;		//Buffers for line mesh
	unsigned int lineVBO;
};