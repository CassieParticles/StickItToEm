#version 330 core

layout(location=0) in vec2 vertexPosition;

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};


uniform vec2 position;	//Centre position
uniform float radius;

//ivec2 gridSize=ivec2(60,35);

out vec2 worldSpace;	//World space of the vertex, used to get world space of fragment, to calculate intensity

void main()
{
	//Vertex offset from centre
	vec2 offset=(vertexPosition-0.5)*2*radius;
	//Get the position of this vertex in world space, also passes this value to the fragment shader, to be used to calculate distance from centre
	worldSpace = position + offset;

	//Position scaled 0-1
	vec2 vertexPos=worldSpace/vec2(gridSize);
	vertexPos.y=1-vertexPos.y;	//Flip y position since it isn't in sync with terrain

	gl_Position=vec4(vertexPos*2-1,0,1);	//Re-scale between -1 and 1
}