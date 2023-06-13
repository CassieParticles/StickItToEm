#include "TerrainManager.h"
#include <glad/glad.h>

TerrainManager::TerrainManager(glm::ivec2 arenaSize):arenaSize{arenaSize}
{
	//Arena size is the amount of squares wide/tall it is, since the scalarData doesn't include the edges that will default to -1, it is 1 unit smaller in both dimensions
	scalarDataSize = (arenaSize.x - 1) * (arenaSize.y - 1);	
	scalarData = new float[scalarDataSize];

	lineArray = new line[arenaSize.x * arenaSize.y * 2];

	glGenVertexArrays(1,&triangleVAO);	//Create the VAO and buffer for the triangles in the terrain, and allocate the memory needed for them
	glBindVertexArray(triangleVAO);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle) * 4 * arenaSize.x * arenaSize.y, nullptr, GL_DYNAMIC_DRAW);	//size of buffer = size of triangle * 4 triangles per square * number of squares
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &lineVAO);	//Create the VAO and buffer for the lines in the terrain, and allocate the memory needed for them
	glBindVertexArray(lineVAO);

	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line) * 2 * arenaSize.x * arenaSize.y, nullptr, GL_DYNAMIC_DRAW);	//size of buffer = size of line * 2 lines per square * number of squares
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);
}

TerrainManager::~TerrainManager()
{
	delete[] scalarData;	//Deleta arrays
	delete[] lineArray;

	glDeleteBuffers(1, &triangleVBO);	//Delete buffers
	glDeleteBuffers(1, &lineVBO);

	glDeleteVertexArrays(1, &triangleVAO);	//Delete VAOs
	glDeleteVertexArrays(1, &lineVAO);
}