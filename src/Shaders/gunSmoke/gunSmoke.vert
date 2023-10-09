#version 330 core

layout(location=0) in vec2 vertexPosition;

uniform vec2 position;	//Centre position
uniform float radius;

out vec2 textCoords;

void main()
{
	textCoords=vertexPosition;


	gl_Position=vec4(vertexPosition*2-1,0,1);
}