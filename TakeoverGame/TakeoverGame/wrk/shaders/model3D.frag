#version 330 core

#define MAX_POINT_LIGHTS 16u
#define MAX_DRCTN_LIGHTS 4u

in vec2 moveUV;
in vec3 movePos;
in vec3 moveNorm;

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
uniform vec3 uViewPos;

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
	//ambient color
	vec3 ambCol = uAmbient * (uAmbientColor * uAmbientIntensity);

	//cumulative diffuse color from all lights
	vec3 difCol = vec3(0.0, 0.0, 0.0);

	//cumulative specular color from all lights
	vec3 specCol = vec3(0.0, 0.0, 0.0);

	vec3 diffAddCol = uDiffuse * texture(tDiffuse, moveUV).xyz;
	vec3 specAddCol = uSpecular * texture(tSpecular, moveUV).xyz;

	for (uint i = 0u; i < MAX_POINT_LIGHTS; i++)
	{
		//difference between surface and light
		vec3 diff = uPointLocation[i] - movePos;

		//adjusted normal of surface
		vec3 norm = normalize(uPointLocation[i] - moveNorm);

		//distance between light and surface
		float dist = length(diff);

		//direction towards light
		vec3 dir = normalize(diff);

		//direction the surface is being viewed from
		vec3 viewDir = normalize(uViewPos - movePos);

		//reflected normal
		vec3 reflectDir = reflect(-dir, norm);

		//calculate specular strength
		float specular = pow(max(dot(viewDir, reflectDir), 0.0), uExponent);

		//calculate angle between surface and light
		float angle = max(dot(norm, dir), 0.0);

		//diffuse strength of the point light at current distance
		float attenuation = 1.0 / (1.0 + (uPointIntensity[i].y * dist) + (uPointIntensity[i].x * dist * dist));

		//add diffuse color
		difCol += diffAddCol * (uPointColor[i] * attenuation * uPointIntensity[i].z * angle);

		//add specular color
		specCol += specAddCol * (uPointColor[i] * specular);
	}

	//combine all colors
	outColor = vec4(ambCol + difCol + specCol, texture(tAlpha, moveUV).w);
}