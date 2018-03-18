#version 400

in vec3 vertex;
out vec3 vxColor;

uniform mat4 m_mat;
uniform mat4 v_mat;
uniform mat4 p_mat;
uniform vec3 m_color;

void main(void)
{
    gl_Position = p_mat*v_mat*m_mat*vec4(vertex, 1.0);
    vxColor = m_color;
}
