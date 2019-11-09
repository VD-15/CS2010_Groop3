#version 330 core

//Texture2D renderer

in vec2 inPosition;
in float inDepth;
in vec4 inColor;

out vec4 moveColor;

uniform mat4 uViewport;

void main()
{
	gl_Position = uViewport * vec4(inPosition, inDepth, 1.0);

	moveColor = inColor;
	moveTextureUV = inTextureUV;
} 