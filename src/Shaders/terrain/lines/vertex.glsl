#version 330 core

layout(location=0) in vec2 position;

uniform vec2 gridSize;

void main()
{
	vec2 cellSize=2/gridSize;	//Inverse of grid size, doubled because positions go between -1 to 1, so range of 2
	vec2 actualCellSize=vec2(2/gridSize.x,2/gridSize.y);

	vec2 transformedPos=vec2(position.x*actualCellSize.x,2-position.y*actualCellSize.y)-1;	//Transforms positions to be between 0 and 2, then subtracts 1
	gl_Position=vec4(transformedPos,0,1);
}