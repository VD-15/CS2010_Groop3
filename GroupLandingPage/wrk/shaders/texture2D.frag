#version 330 core

in vec4 moveColor;
in vec2 moveUV;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 tempColor = texture(uTexture, moveUV) * moveColor;

	if (tempColor.a < 0.001) discard;

    outColor = tempColor;
}