#version 330 core

in vec2 textCoord;

uniform sampler2D spriteSheet;

out vec4 fragColour;

void main()
{
	fragColour=vec4(texture(spriteSheet,textCoord));
}

