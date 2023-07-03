#version 330 core

in vec2 textCoord;

out vec4 fragColour;

void main()
{
	fragColour=vec4(textCoord,0,1);
}

