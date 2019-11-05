#version 330 core

//Texture2D renderer

in vec2 inPosition;
in float inDepth;
in vec4 inColor;
in vec2 inTextureUV;

out vec4 moveColor;
out vec2 moveTextureUV;

uniform mat4 uViewport;

void main()
{
	gl_Position = uViewport * vec4(inPosition, inDepth, 1.0);

	moveColor = inColor;
	moveTextureUV = inTextureUV;
} 