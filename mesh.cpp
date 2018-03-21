#include "mesh.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

Mesh::Mesh()
{
    //setup mesh gl
    m_ref.m_vao = m_ref.m_vertCount = m_ref.m_vert_vbo = 0;
    s_ref.prog = 0;
}

Mesh::~Mesh()
{
    cleanVAO();
    s_ref.prog = 0;
    m_ref.m_vao = 0;
}

// ---------------------------------------------------------------

void Mesh::Cube()
{
    vertices.clear(); indices.clear(); uvs.clear();
    normals.clear(); faces.clear();

    //simple cube mesh
    glm::vec3 cube_vertices[] = {
        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,-0.5f),

        glm::vec3(-0.5f,0.5f,0.5f),
        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),

        glm::vec3(0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),

        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(-0.5f,0.5f,0.5f),

        glm::vec3(-0.5f,0.5f,0.5f),
        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,0.5f),

        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,0.5f)
    };

    GLuint cube_indices []= {
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
    };

    vertices = std::vector<glm::vec3>(cube_vertices, cube_vertices + sizeof(cube_vertices) / sizeof(glm::vec3));
    indices = std::vector<GLuint>(cube_indices, cube_indices + sizeof(cube_indices) / sizeof(GLuint));
}

void Mesh::loadMesh()
{
    createVAO();
    bindVAO();
    pushToVAO();
    unbindVAO();
}

void Mesh::renderMe()
{
    if(indices.size()){
        GLctx->glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, (void*)0);
    } else GLctx->glDrawArrays(GL_TRIANGLES, 0, m_ref.m_vertCount);
}

// ---------------------------------------------------------------

void Mesh::createVAO()
{
    GLctx->glGenVertexArrays(1, &m_ref.m_vao);
}

void Mesh::pushToVAO()
{
    if(vertices.size() > 0){
        GLctx->glGenBuffers(1, &m_ref.m_vert_vbo);
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, m_ref.m_vert_vbo);
        GLctx->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        GLctx->glVertexAttribPointer(VERTEX_VAO_IDX, 3, GL_FLOAT, GL_FALSE, 0, 0);
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(indices.size() > 0){
        GLctx->glGenBuffers(1, &m_ref.m_idx_vbo);
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ref.m_idx_vbo);
        GLctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::bindVAO()
{
    GLctx->glBindVertexArray(m_ref.m_vao);
    if(indices.size() > 0) GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ref.m_idx_vbo);
    GLctx->glEnableVertexAttribArray(VERTEX_VAO_IDX);
}

void Mesh::unbindVAO()
{
    GLctx->glDisableVertexAttribArray(VERTEX_VAO_IDX);
    GLctx->glBindVertexArray(0);
    if(indices.size() > 0) GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::cleanVAO()
{
    unbindVAO();
    GLctx->glDeleteVertexArrays(1, &m_ref.m_vao);
    GLctx->glDeleteBuffers(1, &m_ref.m_vert_vbo);
    GLctx->glDeleteBuffers(1, &m_ref.m_idx_vbo);
}

// ---------------------------------------------------
