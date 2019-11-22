#version 330 core

//per vertex attributes
in vec3 inPos;
in vec2 inUV;
in vec3 inNormal;

//per instance attributes
in mat4 inTransform;

out vec2 moveUV;
out vec3 movePos;

uniform mat4 uViewport;

void main()
{
    vec4 worldPos = (inTransform * vec4(inPos, 1.0));

    gl_Position = uViewport * worldPos;
    moveUV = inUV;
    movePos = worldPos.xyz / worldPos.w;
}