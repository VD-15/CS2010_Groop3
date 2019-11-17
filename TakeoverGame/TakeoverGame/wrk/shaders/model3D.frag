#version 330 core

in vec2 moveUV;

out vec4 outColor;

//Material info
uniform vec4 uAmbient;
uniform vec4 uDiffuse;
uniform vec4 uSpecular;
uniform float uExponent;
uniform float uAlpha;

//Material textures
uniform sampler2D tDiffuse;		//set to texture unit 0
uniform sampler2D tSpecular;	//set to texture unit 1
uniform sampler2D tExponent;	//set to texture unit 2
uniform sampler2D tAlpha;		//set to texture unit 3

void main()
{
	//just use diffuse color for now
	outColor = uDiffuse;
}