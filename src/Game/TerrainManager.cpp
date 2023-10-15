#include "TerrainManager.h"

#include <glad/glad.h>
#include <algorithm>

#include <Engine/MathsFunctions.h>
#include <Engine/Program.h>
#include <Engine/Collision.h>

TerrainManager::TerrainManager(glm::ivec2 arenaSize, Gunsmoke* gunSmokeManager) :arenaSize{ arenaSize }, gunSmokeManager{ gunSmokeManager}
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

	triangleProgram = new Program("src/Shaders/terrain/triangles/terrainTriangles.vert","src/Shaders/terrain/triangles/terrainTriangles.frag",Program::filePath);
	lineProgram = new Program("src/Shaders/terrain/lines/terrainLines.vert", "src/Shaders/terrain/lines/terrainLines.frag",Program::filePath);

	triangleProgram->setUniformBufferBlockBinding("terrainData", 1);
	lineProgram->setUniformBufferBlockBinding("terrainData", 1);

	bgTexture = TextureManager::getTexturePtr("assets/troll.png");
}

TerrainManager::~TerrainManager()
{
	delete[] scalarData;	//Deleta arrays
	delete[] lineArray;

	glDeleteBuffers(1, &triangleVBO);	//Delete buffers
	glDeleteBuffers(1, &lineVBO);

	glDeleteVertexArrays(1, &triangleVAO);	//Delete VAOs
	glDeleteVertexArrays(1, &lineVAO);

	delete triangleProgram;
	delete lineProgram;
}

void TerrainManager::uploadStage(float* stage)
{
	std::copy(stage, stage + scalarDataSize, scalarData);

	generateTerrain(-1,-1,60,35);
}

void TerrainManager::modifyTerrainCircle(glm::vec2 centre, float radius, float value)
{
	glm::ivec2 tl = glm::ivec2{ floor(centre.x - radius),floor(centre.y - radius) };	//Get the bounds of the area affected
	glm::ivec2 br = glm::ivec2{ ceil(centre.x + radius),ceil(centre.y + radius) };

	float invRSqr = 1 / (radius * radius);	//Pre-calculate the inverse of the radius square

	for (int y = tl.y; y < br.y; y++)	//Iterate through each point of the area
	{
		for (int x = tl.x; x < br.x; x++)
		{
			float sqrDistFromCentre = (x - centre.x) * (x - centre.x) + (y - centre.y) * (y - centre.y);	//Get the square distance
			
			float scalar = std::max(0.f,1 - (sqrDistFromCentre)*invRSqr);	//Get the fraction of the value added to the point

			addPoint({ x,y }, scalar * value);
		}
	}

	generateTerrain(tl.x, tl.y, br.x - tl.x, br.y - tl.y);	//Regenerate terrain in area
}

void TerrainManager::generateTerrain(int left, int top, int width, int height)
{
	left = std::min(std::max(-1, left), arenaSize.x - 1);	//Clamp left and top to be within bounds of terrain
	top = std::min(std::max(-1, top), arenaSize.y - 1);

	width = std::min(std::max(1, width), arenaSize.x - 1 - left);	//Clamp width and height to fit in terrain
	height = std::min(std::max(1, height), arenaSize.y - 1 - top);

	//Which vertices are used in the triangles for that square
	constexpr char vertexTable[16]{	
		0b00000000,	//0
		0b00000111,
		0b00011100,
		0b00011011,
		0b01110000,
		0b01110111,	//5
		0b01101100,
		0b01101011,
		0b11000001,
		0b11000110,
		0b11011101,	//10
		0b11011010,
		0b10110001,
		0b10110110,
		0b10101101,
		0b10101010	//15
	};

	//How the vertices are assembled in clockwise order, starting at the MSB, upto 4 triangles, 8 means no more triangles
	constexpr char triangleIndexTable[16][13]{
		{8,8,8,8,8,8,8,8,8,8,8,8,8},//0
		{2,1,0,8,8,8,8,8,8,8,8,8,8},
		{4,3,2,8,8,8,8,8,8,8,8,8,8},
		{4,3,0,3,1,0,8,8,8,8,8,8,8},
		{6,5,4,8,8,8,8,8,8,8,8,8,8},
		{6,5,4,6,4,2,6,2,0,2,1,0,8},//5
		{6,5,3,6,3,2,8,8,8,8,8,8,8},
		{6,5,3,6,3,0,3,1,0,8,8,8,8},
		{7,6,0,8,8,8,8,8,8,8,8,8,8},
		{7,6,2,7,2,1,8,8,8,8,8,8,8},
		{7,6,0,6,4,2,6,2,0,4,3,2,8},//10
		{7,6,1,6,4,1,4,3,1,8,8,8,8},
		{7,5,4,7,4,0,8,8,8,8,8,8,8},
		{7,5,4,7,4,2,7,2,1,8,8,8,8},
		{7,5,0,5,2,0,5,3,2,8,8,8,8},
		{7,5,3,7,3,1,8,8,8,8,8,8,8}	//15
	};

	//How the lines are assembled, in order so that for vector B-A, "left" of that direction is air and "right" is the solid terrain
	constexpr char lineIndexTable[16][5]{
		{8,8,8,8,8},//0
		{0,2,8,8,8},
		{2,4,8,8,8},
		{0,4,8,8,8},
		{4,6,8,8,8},
		{0,6,4,2,8},//5
		{2,6,8,8,8},
		{0,6,8,8,8},
		{6,0,8,8,8},
		{6,2,8,8,8},
		{6,4,2,0,8},//10
		{6,4,8,8,8},
		{4,0,8,8,8},
		{4,2,8,8,8},
		{2,0,8,8,8},
		{8,8,8,8,8}	//15
	};

	//Offsets for each corner
	constexpr glm::vec2 offsets[4]{
		{0,1},
		{1,1},
		{1,0},
		{0,0}
	};

	//Array to contain the triangles and lines 
	triangle* triangleArray = new triangle[4 * width * height];	
	line* tempLineArray = new line[2 * width * height];

	int squareIndex = 0;

	for (int y = top; y < top + height; y++)
	{
		for (int x = left; x < left + width; x++)
		{
			float cornerValues[4]	//Get the floating point values within each corner
			{
				getPoint(glm::vec2{x,y} + offsets[0]),		//(0,1) BL
				getPoint(glm::vec2{x,y} + offsets[1]),		//(1,1) BR
				getPoint(glm::vec2{x,y} + offsets[2]),		//(1,0) TR
				getPoint(glm::vec2{x,y} + offsets[3]),		//(0,0) TL
			};

			int index{};	//Get the index of the correct configuration of triangles and lines
			for (int i = 3; i >= 0; i--)
			{
				index = (index << 1) | cornerValues[i] > 0;
			}

			glm::vec2 corners[4]	//Get the co-ordinates of each corner (offset by 1 so the actual positions start at 0)
			{
				glm::vec2{x + 1,y + 1} + offsets[0],
				glm::vec2{x + 1,y + 1} + offsets[1],
				glm::vec2{x + 1,y + 1} + offsets[2],
				glm::vec2{x + 1,y + 1} + offsets[3],
			};

			const char vertexMask = vertexTable[index];	//Get which vertices are used in the final output
			const char* triangleIndices = triangleIndexTable[index];	//Get the assembly order of the vertices when making triangles and lines
			const char* lineIndices = lineIndexTable[index];

			glm::vec2 vertices[8]{};	//Get the vertices, only fills in the ones that are used

			if (vertexMask & 0b00000001) { vertices[0] = MathsFunctions::getMidPtY(corners[3], corners[0], cornerValues[3], cornerValues[0]); }
			if (vertexMask & 0b00000010) { vertices[1] = corners[0]; }
			if (vertexMask & 0b00000100) { vertices[2] = { MathsFunctions::getMidPtX(corners[0],corners[1],cornerValues[0],cornerValues[1]) }; }
			if (vertexMask & 0b00001000) { vertices[3] = corners[1]; }
			if (vertexMask & 0b00010000) { vertices[4] = { MathsFunctions::getMidPtY(corners[1],corners[2],cornerValues[1],cornerValues[2]) }; }
			if (vertexMask & 0b00100000) { vertices[5] = corners[2]; }
			if (vertexMask & 0b01000000) { vertices[6] = { MathsFunctions::getMidPtX(corners[2],corners[3],cornerValues[2],cornerValues[3]) }; }
			if (vertexMask & 0b10000000) { vertices[7] = corners[3]; }

			for (int i = 0; i < 4; i++)
			{
				triangle t{};
				if (triangleIndices[i * 3] != 8)
				{
					t.A = vertices[triangleIndices[i * 3 + 0]];	//Get each vertex of the triangle
					t.B = vertices[triangleIndices[i * 3 + 1]];
					t.C = vertices[triangleIndices[i * 3 + 2]];
				}
				triangleArray[squareIndex * 4 + i] = t;		//Add the triangle to the array
			}

			for (int i = 0; i < 2; i++)	//Assemble the lines, lines need to be overwritten, so can't stop early
			{
				line l{};
				if (lineIndices[i * 2] == 8) { l = {}; }	//If there is no line, make it {0,0} to {0,0}
				else
				{
					l.A = vertices[lineIndices[i * 2 + 0]];	//If there is a line, assemble the line
					l.B = vertices[lineIndices[i * 2 + 1]];
				}
				tempLineArray[squareIndex * 2 + i] = l;
			}

			squareIndex++;
		}
	}

	/*Overwrite buffer for triangles
	* Rows are stored contigously, columns are not
	* Calculate index of first triangle of row
		* index=(y*arenaSize.width+x)*96 bytes
			* y will increment with each row, x will be consistent
	* Overwrite next [width*4] triangles with [width*4] triangles in array
	*	number of bytes=width * 4 triangles * 3 vertices * 2 floats * 4 bytes = width * 96 bytes
	*/

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	triangle* trianglePtr=triangleArray;

	for (int y = 0; y < height; y++)	//Iterate through each row, put the data for that row into the buffer, then move the pointer
	{
		int indexOffset = (((y + top + 1) * arenaSize.x) + left + 1) * 4;
		glBufferSubData(GL_ARRAY_BUFFER, indexOffset*sizeof(triangle), sizeof(triangle) * 4 * width, trianglePtr);
		trianglePtr += width * 4;
	}

	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	line* linePtr = tempLineArray;

	for (int y = 0; y < height; y++)
	{
		int indexOffset = (((y + top + 1) * arenaSize.x) + left + 1) * 2;
		glBufferSubData(GL_ARRAY_BUFFER, indexOffset * sizeof(line), sizeof(line) * 2 * width, linePtr);

		std::copy(linePtr, linePtr + 2 * width, lineArray + indexOffset);

		linePtr += width * 2;
	}

	delete[] triangleArray;
	delete[] tempLineArray;
}

float TerrainManager::getPoint(glm::ivec2 pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= arenaSize.x-1 || pos.y >=arenaSize.y-1)	//If value is outside the bounds of the arena, return
	{
		return -1;
	}

	int index = (arenaSize.y-2-pos.y) * (arenaSize.x - 1) + pos.x;
	return scalarData[index];
}

void TerrainManager::setPoint(glm::ivec2 pos, float val)
{
	val = std::min(std::max(-1.f, val), 1.f);	//Clamp val between +1 and -1

	if (pos.x < 0 || pos.x > arenaSize.x - 1) { return; }	//If point is outside of bounds, return
	if (pos.y < 0 || pos.y < arenaSize.y - 1) { return; }

	scalarData[(arenaSize.y - 2 - pos.y) * (arenaSize.x - 1) + pos.x] = val;
}

void TerrainManager::addPoint(glm::ivec2 pos, float val)
{
	if (pos.x < 0 || pos.x > arenaSize.x - 2) { return; }	//If point is outside of bounds, return
	if (pos.y < 0 || pos.y > arenaSize.y - 2) { return; }

	int index = (arenaSize.y - pos.y - 2) * (arenaSize.x - 1) + pos.x;	//Get the index of the point being added to

	float newVal = scalarData[index] + val;	//Get the value, clamp it, then set it
	newVal = std::min(std::max(-1.f, newVal), 1.f);
	scalarData[index] = newVal;
}

line* TerrainManager::getLines(glm::ivec2 tl, int* lineCount)
{
	line* lines;
	if (tl.x<0 || tl.y<0 || tl.x>arenaSize.x-1 || tl.y>arenaSize.y-1) { return nullptr; }
	int squareIndex = (tl.y) * (arenaSize.x) + tl.x;	//Gets which square to access (not the index in the array)
	lines = new line[2];
	lines[0] = lineArray[squareIndex * 2 + 0];
	lines[1] = lineArray[squareIndex * 2 + 1];

	*lineCount = 2;
	return lines;
}

line* TerrainManager::getLines(glm::ivec2 tl, glm::ivec2 area, int* lineCount)
{
	tl.x = std::clamp(tl.x, 0, arenaSize.x); //Make sure co-ords are positive but less then the arenaSize
	tl.y = std::clamp(tl.y, 0, arenaSize.y);

	area.x = std::clamp(area.x, 0, arenaSize.x - tl.x);		//Ensure area is positive
	area.y = std::clamp(area.y, 0, arenaSize.y - tl.y);

	*lineCount = area.x * area.y * 2;	//Calculate number of lines gotten (includes non-lines)
	line* lines=new line[area.x * area.y * 2];

	int squareRowIndex = (tl.y) * arenaSize.x + tl.x;	//As y index is iterated through, the index will decrease
	
	for (int y = 0; y < area.y; y++)
	{
		int squareIndex = squareRowIndex;
		for (int x = 0; x < area.x; x++)
		{
			lines[(y*area.x+x)*2+0] = lineArray[squareIndex * 2 + 0];	//Copy data over
			lines[(y*area.x+x)*2+1] = lineArray[squareIndex * 2 + 1];
			squareIndex++;
		}


		squareRowIndex += arenaSize.x;	//Increase squareIndex by one row
	}
	
	return lines;
}

void TerrainManager::displayLines()	//Debug function
{
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line) * arenaSize.x * arenaSize.y * 2, lineArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TerrainManager::render()
{
	//Render the triangles in the terrain
	triangleProgram->use();	

	triangleProgram->setVec3("colour", glm::vec3{ 0,0,0 });
	triangleProgram->setInt("bgTexture", 0);
	triangleProgram->setInt("gunsmokeTexture", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bgTexture.textureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gunSmokeManager->getTexture());

	glBindVertexArray(triangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 12 * arenaSize.x * arenaSize.y);
	
	//Render the lines in the terrain
	lineProgram->use();
	lineProgram->setVec3("colour", glm::vec3{ 1,0,0 });

	glBindVertexArray(lineVAO);
	glDrawArrays(GL_LINES, 0, 4 * arenaSize.x * arenaSize.y);

	glBindVertexArray(0);
}