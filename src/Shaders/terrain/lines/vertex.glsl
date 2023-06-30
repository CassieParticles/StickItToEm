#version 330 core

layout(location=0) in vec2 position;

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

void main()
{
	vec2 cellSize=2/vec2(gridSize);	//Inverse of grid size, doubled because positions go between -1 to 1, so range of 2

	vec2 transformedPos=vec2(position.x*cellSize.x,2-position.y*cellSize.y)-1;	//Transforms positions to be between 0 and 2, then subtracts 1
	gl_Position=vec4(transformedPos,0,1);
}