#version 330 core

//Primitive polygon renderer

in vec2 inPosition;
in float inDepth;
in vec4 inColor;

uniform mat4 uViewport;

out vec4 moveColor;

void main()
{
	gl_Position = uViewport * vec4(inPosition, inDepth, 1.0);
	moveColor = inColor;
}