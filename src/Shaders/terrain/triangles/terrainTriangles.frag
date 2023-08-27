#version 330 core

in vec2 arenaPos;
in vec2 texCoord;

uniform sampler2D bgTexture;

uniform vec3 colour;

out vec4 outColour;

void main()
{
	outColour=vec4(texture(bgTexture,texCoord).xyz,1);
}