#version 330

out vec4 outCol;

in vec2 moveTex;
in vec3 moveNml;
in vec3 movePos;

//Directional light
uniform vec3 dirLightPos;
uniform vec3 dirLightCol;
uniform float dirLightInt;

//Ambient light
uniform vec3 ambLightCol;
uniform float ambLightInt;

//Camera position
uniform vec3 cameraPos;

//Material information
uniform vec3 ambMaterialCol;
uniform vec3 difMaterialCol;
uniform vec3 spcMaterialCol;
uniform float spcMaterialExp;
uniform float materialAlp;

uniform sampler2D ambTex; //0
uniform sampler2D difTex; //1
uniform sampler2D spcTex; //2
uniform sampler2D alpTex; //3

float ColToBW(vec3 col)
{
    return (col.x + col.y + col.z) / 3.0;
}

void main()
{
    //Ambient light
    vec3 ambientColor = ambLightCol * ambLightInt * ambMaterialCol * texture(ambTex, moveTex).xyz;

    //Directional light
    float dirDiffuseAmount = max(dot(moveNml, normalize(dirLightPos)), 0.0);
    vec3 directionalColor = dirDiffuseAmount * dirLightInt * dirLightCol * difMaterialCol * texture(difTex, moveTex).xyz;

    //Directional light specular highlight
    vec3 viewDir = normalize(cameraPos - movePos);
    vec3 dirReflectDir = reflect(-dirLightPos, moveNml);
    float dirSpecularAmount = pow(max(dot(viewDir, dirReflectDir), 0.0), spcMaterialExp);
    float specularBrightness = ColToBW(texture(spcTex, moveTex).xyz);
    vec3 directionalSpecular = dirSpecularAmount * dirLightInt * dirLightCol * spcMaterialCol * specularBrightness;

	//outCol = vec4(1.0, 1.0, 1.0, 1.0);
	//outCol = vec4(directionalColor, 1.0);
    outCol = vec4(ambientColor + directionalColor + directionalSpecular, materialAlp * ColToBW(texture(alpTex, moveTex).xyz));
}