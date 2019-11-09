#version 330 core

in vec4 moveColor;
in vec2 moveTextureUV;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 tempColor = moveColor * texture(uTexture, moveTextureUV);

	if (tempColor.a < 0.001) discard;

	outColor = tempColor;
}