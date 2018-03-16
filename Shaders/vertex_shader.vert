#version 400

in vec3 vertex;
out vec3 vxColor;

void main(void)
{
    gl_Position = vec4(vertex/2.0, 1.0);
    gl_Position.x += 0.3;
    vxColor = vec3(1.0, 1.0, 1.0);
}
