#version 330 core

#define MAX_POINT_LIGHTS 64u
#define MAX_DRCTN_LIGHTS 4u

in vec2 moveUV;
in vec3 movePos;

out vec4 outColor;

//Material info
uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
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
uniform uint uPointCount;

//Directional lights
uniform vec3 uDirectionColor[MAX_DRCTN_LIGHTS];
uniform vec3 uDirectionIntensity[MAX_DRCTN_LIGHTS];
uniform vec3 uDirectionVector[MAX_DRCTN_LIGHTS];
uniform uint uDirectionCount;

void main()
{
	//just use diffuse color for now
	vec3 ambCol = uAmbient * (uAmbientColor * uAmbientIntensity);
	vec3 difCol = vec3(0.0, 0.0, 0.0);

	for (uint i = 0u; i < MAX_POINT_LIGHTS; i++)
	{
		float dist = length(uPointLocation[i] - movePos);
		float attenuation = 1.0 / (1.0 + (uPointIntensity[i].y * dist) + (uPointIntensity[i].x * dist * dist));
		difCol += uDiffuse * (uPointColor[i] * attenuation * uPointIntensity[i].z);
	}

	//outColor = uDiffuse * vec4((uAmbientColor * uAmbientIntensity), 1.0);
	outColor = vec4(ambCol + difCol, 1.0);
}