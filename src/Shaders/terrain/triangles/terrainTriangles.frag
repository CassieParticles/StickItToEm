#version 330 core

in vec2 arenaPos;
in vec2 texCoord;

uniform sampler2D bgTexture;
uniform sampler2D gunsmokeTexture;

uniform vec3 colour;

out vec4 outColour;

void main()
{
//	vec4 gunSmoke= vec4(0.6,0.0,0.8,1.0);
	vec4 gunSmoke=texture(gunsmokeTexture,texCoord);
	outColour=vec4(texture(bgTexture,texCoord).xyz * (1-gunSmoke.w) + gunSmoke.xyz * gunSmoke.w,1);
}