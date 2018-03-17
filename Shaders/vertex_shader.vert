#version 400

in vec3 vertex;
out vec3 vxColor;

uniform mat4 m_mat;

void main(void)
{
    gl_Position = m_mat*vec4(vertex, 1.0);
    vxColor = vec3(1.0, 1.0, 1.0);
}
