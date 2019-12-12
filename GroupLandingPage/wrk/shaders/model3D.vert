#version 330

out vec2 moveTex;
out vec3 moveNml;
out vec3 movePos;

in vec3 inPos;
in vec2 inTex;
in vec3 inNml;

uniform mat4 uViewport;
in mat4 inTransform;
in mat4 inRotation;

void main()
{
    vec4 worldPos = inTransform * vec4(inPos, 1.0);
    vec4 worldNml = inRotation * vec4(inNml, 1.0);
    gl_Position = uViewport * worldPos;

    moveTex = inTex;
    moveNml = worldNml.xyz;
    movePos = worldPos.xyz;
}