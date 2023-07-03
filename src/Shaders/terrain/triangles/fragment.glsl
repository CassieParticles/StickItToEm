#version 330 core

in vec2 arenaPos;

uniform vec3 colour;

out vec4 outColour;

void main()
{
	outColour=vec4(fract(arenaPos),1,1);
}