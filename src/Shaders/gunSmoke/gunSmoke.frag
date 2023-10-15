#version 330 core

in vec2 worldSpace;	//Can also be used to get position on screen from 0-1

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

uniform vec2 position;	//Centre position
uniform float radius;

uniform vec3 colour;

out vec4 fragColour;

void main()
{

	vec2 disp=position-worldSpace;	//Get the scalar distance between the place in the game and the centre of the gunsmoke
	float distSqr=dot(disp,disp);	

	float scalarSqr=1.0-min(1.0,distSqr/(radius*radius));	//Get a scalar from 0-1 for the distance, 1 being the centre, 0 being outside the circle (polynomial falloff)

	fragColour=vec4(colour,scalarSqr);	//Should use alpha blending to blend the colours
}