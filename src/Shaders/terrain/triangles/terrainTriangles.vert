#version 330 core

layout(location=0) in vec2 position;

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

out vec2 arenaPos;
out vec2 texCoord;

void main()
{
	arenaPos=position;
	texCoord=vec2(position.x/gridSize.x,position.y/gridSize.y);
	texCoord.y=1-texCoord.y;

	vec2 gridSizeFloat=vec2(gridSize);
	vec2 cellSize=2/vec2(gridSize);	//Inverse of grid size, doubled because positions go between -1 to 1, so range of 2

	vec2 transformedPos=vec2(position.x*cellSize.x,position.y*cellSize.y)-1;	//Transforms positions to be between 0 and 2, then subtracts 1
	gl_Position=vec4(transformedPos,0,1);
}