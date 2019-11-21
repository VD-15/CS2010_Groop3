#version 330 core

#define MAX_POINT_LIGHTS 64
#define MAX_DRCTN_LIGHTS 4

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

//Lights

//Ambient light
uniform vec3  uAmbientColor;
uniform float uAmbientIntensity;

//Point lights
uniform vec3 uPointColor[MAX_POINT_LIGHTS];
uniform vec3 uPointIntensity[MAX_POINT_LIGHTS];
uniform vec3 uPointLocation[MAX_POINT_LIGHTS];

//Directional lights
uniform vec3 uDirectionColor[MAX_DRCTN_LIGHTS];
uniform vec3 uDirectionIntensity[MAX_DRCTN_LIGHTS];
uniform vec3 uDirectionVector[MAX_DRCTN_LIGHTS];

void main()
{
	//just use diffuse color for now
	outColor = uDiffuse * vec4((uAmbientColor * uAmbientIntensity), 1.0);
}