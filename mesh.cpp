#include "mesh.h"
#include "qopengl.h"

Mesh::Mesh()
{
    //setup mesh gl
    ref.m_vao = ref.m_vertCount = ref.m_pos_vbo = 0;
    shaders.m_frag_shader = shaders.m_vertex_shader = nullptr;
    shaders.m_shader_prog = nullptr;


    //simple quad mesh
    vertices.push_back(vertex(-0.3, -0.7, 0.0));
    vertices.push_back(vertex(0.5, 0.5, 0.0));
    vertices.push_back(vertex(-0.5, 0.5, 0.0));
    vertices.push_back(vertex(-0.5, -0.5, 0.0));
    vertices.push_back(vertex(0.5, -0.5, 0.0));
    vertices.push_back(vertex(0.5, 0.5, 0.0));
    ref.m_vertCount = vertices.size();
}

Mesh::~Mesh()
{
    unbindVAO();
    gl->glDeleteVertexArrays(1, &ref.m_vao);
    gl->glDeleteBuffers(1, &ref.m_pos_vbo);
}

void Mesh::loadMesh()
{
    createVAO();
    pushToVAO();
    unbindVAO();
}

void Mesh::loadShaders()
{

}

void Mesh::createVAO()
{
    gl->glGenVertexArrays(1, &ref.m_vao);
    bindVAO();
}

void Mesh::bindVAO()
{
    gl->glBindVertexArray(ref.m_vao);
}

void Mesh::unbindVAO()
{
    gl->glBindVertexArray(0);
}

void Mesh::pushToVAO()
{
    gl->glGenBuffers(1, &ref.m_pos_vbo);
    gl->glBindBuffer(GL_ARRAY_BUFFER, ref.m_pos_vbo);
    gl->glBufferData(GL_ARRAY_BUFFER, ref.m_vertCount * sizeof(vertex), &vertices[0], GL_DYNAMIC_DRAW);
    gl->glVertexAttribPointer(POS_VAO_IDX, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
