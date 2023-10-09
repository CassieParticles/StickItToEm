#version 330 core

in vec2 textCoord;	//Can also be used to get position on screen from 0-1

layout(std140) uniform terrainData
{
	ivec2 gridSize;
};

uniform sampler2D readTexture;

uniform vec2 position;	//Centre position
uniform float radius;

uniform vec3 colour;

out vec4 fragColour;

void main()
{
	//Get fragment position in game space
	vec2 gameSpace=textCoord*gridSize;	//Get position of fragment in game space

	vec2 disp=position-gameSpace;	//Get the scalar distance between the place in the game and the centre of the gunsmoke
	float distSqr=dot(disp,disp);	

	float dist=sqrt(distSqr);

	float scalar=max(radius-dist,0) / radius;	//Get the scalar multiple for the colour being applied, lowest value is 0

	vec4 originalColour=texture(readTexture,textCoord);	//Get the colour before, to be blended between

	vec3 outColour=originalColour.xyz*(1-scalar) + colour * scalar;	//Get the blended colour, and the new alpha value (larger between original alpha or scalar)
	float outAlpha = max(originalColour.w,scalar);

	//fragColour=vec4(outColour,outAlpha);	//Draw to write texture
	fragColour=vec4(textCoord,0.5,0.5);
}