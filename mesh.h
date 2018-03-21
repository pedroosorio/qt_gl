#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>

// VAO Indexes
#define VERTEX_VAO_IDX 0

typedef struct _MeshReference{
    GLuint m_vao;
    int32_t m_vertCount;
    GLuint m_vert_vbo;
    GLuint m_idx_vbo;
} MeshReference;

typedef struct _MeshShaderReference{
    GLuint prog;
} MeshShaderReference;

typedef struct _MeshFace{
    glm::vec3 p[3];
} MeshFace;

class Mesh
{
public:
    Mesh();
    ~Mesh();
    // Simple mesh generation functions
    void Cube();

    // Mesh information getters
    inline int32_t getVAO() { return m_ref.m_vao; }
    inline int32_t getNumVerts() { return m_ref.m_vertCount; }
    inline MeshReference getMeshReference() { return m_ref; }
    inline GLuint getShaderProgram() { return s_ref.prog; }

    // VAO utility functions
    void bindVAO();
    void unbindVAO();
protected:
    friend class Model;
    friend class OBJLoader;

    // Mesh - VAO - specific GL functions
    void createVAO();
    void pushToVAO();
    void cleanVAO();
    void loadMesh();

    // Mesh render function
    void renderMe();

    // Mesh data
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<MeshFace> faces;
private:
    // OpenGL context data
    MeshShaderReference s_ref;
    // Mesh reference data
    MeshReference m_ref;
};

#endif // MESH_H
