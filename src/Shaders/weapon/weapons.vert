#version 330 core

layout(location=0) in vec2 vertexPosition;
layout(location=1) in int cornerIndex;

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

uniform vec2 position;
uniform vec2 size;

uniform mat2x2 rotMatrix;

uniform vec2[4] texCoords;	//Uniform around if the animation
uniform int animationFlipped;

out vec2 textCoord;

void main()
{
	//Find the centre, for flipping purposes
	vec2 centre=vec2(size/2)+position;


	vec2 displacedPos=vertexPosition*size-size/2;
	vec2 rotatedPos=displacedPos*rotMatrix;

	vec2 vertexPos=rotatedPos+size/2+position;
	vec2 cellSize=2/vec2(gridSize);


	float dX=vertexPos.x-centre.x;
	vertexPos.x+=-2*dX*animationFlipped;

	vec2 screenPos=vec2(vertexPos.x*cellSize.x,vertexPos.y*cellSize.y)-1;

	gl_Position=vec4(screenPos,0,1);

	int i=cornerIndex;	//Flips the lowest bit if the animation is flipped, which means left-right side is flipped



	textCoord=texCoords[i];
}