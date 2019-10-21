#version 330 core

in vec3 vertPos;
in vec3 vertCol;

out vec3 moveCol;

uniform mat4 uViewport;

void main()
{
    gl_Position = uViewport * vec4(vertPos, 1.0);
    moveCol = vertCol;
}