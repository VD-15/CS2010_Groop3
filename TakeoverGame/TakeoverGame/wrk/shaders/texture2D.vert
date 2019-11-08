#version 330 core

//Texture2D renderer

in vec2 inPosition;
in vec2 inUV;
in float inDepth;
in vec4 inColor;

out vec4 moveColor;
out vec2 moveUV;

uniform mat4 uViewport;

void main()
{
	gl_Position = uViewport * vec4(inPosition, inDepth, 1.0);

	moveColor = inColor;
	moveUV = inUV;
} 