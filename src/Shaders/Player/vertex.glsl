#version 330 core

layout(location=0) in vec2 vertexPosition;
layout(location=1) in int cornerIndex;

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

uniform vec2 position;
uniform vec2 size;

uniform vec2[4] texCoords;	//Uniform around if the animation
uniform int animationFlipped;

out vec2 textCoord;

void main()
{
	
	vec2 playerPos=vertexPosition*size+position;
	vec2 cellSize=2/vec2(gridSize);

	vec2 screenPos=vec2(playerPos.x*cellSize.x,playerPos.y*cellSize.y)-1;

	gl_Position=vec4(screenPos,0,1);

	int i=cornerIndex^animationFlipped;	//Flips the lowest bit if the animation is flipped, which means left-right side is flipped

	textCoord=texCoords[i];
}