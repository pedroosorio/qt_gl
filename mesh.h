#ifndef MESH_H
#define MESH_H

#include "ThirdParty/glm/glm.hpp"
#include <QOpenGLFunctions_4_0_Core>
#include <vector>

#define POS_VAO_IDX 0

typedef glm::vec3 vertex;
typedef glm::vec3 c_rgb;
typedef glm::vec4 c_rgba;

typedef struct MeshReference{
    GLuint m_vao;
    int32_t m_vertCount;
    GLuint m_pos_vbo;
} MeshReference;

class Mesh
{
public:
    Mesh();
    ~Mesh();
    void setOpenGLContext(QOpenGLFunctions_4_0_Core *ctx) {gl = ctx;}

    int32_t getVAO() { return ref.m_vao; }
    int32_t getNumVerts() { return ref.m_vertCount; }
    MeshReference getMeshReference() { return ref; }
    void bindVAO();
    void unbindVAO();

    void loadMesh(float mesh_verts []);
protected:
    void createVAO();
    void pushToVAO();
private:
    // OpenGL mesh context data
    MeshReference ref;
    QOpenGLFunctions_4_0_Core *gl;

    // Mesh data
    std::vector<vertex> vertices;
};

#endif // MESH_H
