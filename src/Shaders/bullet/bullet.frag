#version 330 core

in vec2 textCoord;

uniform sampler2D spriteSheet;

out vec4 fragColour;

void main()
{
	vec4 frag=texture(spriteSheet,textCoord);
	fragColour=vec4(frag.xyz,frag.w);
}

