#version 400 core

in vec3 vxColor;
out vec4 pxColor;

void main(void)
{
    pxColor = vec4(vxColor, 1.0);
}
