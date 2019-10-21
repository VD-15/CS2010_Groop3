#version 330 core

in vec3 moveCol;

out vec4 outCol;

void main()
{
    outCol = vec4(moveCol, 1.0);
}