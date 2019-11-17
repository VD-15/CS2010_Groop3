#version 330 core

//per vertex attributes
in vec3 inPos;
in vec2 inUV;
in vec3 inNormal;

//per instance attributes
in mat4 inTransform;

out vec2 moveUV;

uniform mat4 uViewport;

void main()
{
    gl_Position = uViewport * (inTransform * vec4(inPos, 1.0));
    moveUV = inUV;
}