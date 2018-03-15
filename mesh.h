#ifndef MESH_H
#define MESH_H

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/quaternion.hpp"
#include "ThirdParty/glm/gtx/quaternion.hpp"
#include "ThirdParty/glm/gtx/euler_angles.hpp"
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <vector>

#define POS_VAO_IDX 0

typedef glm::vec3 pos;
typedef glm::vec3 rot;
typedef glm::quat orient;
typedef glm::vec3 vertex;
typedef glm::vec3 c_rgb;
typedef glm::vec4 c_rgba;

typedef struct MeshReference{
    GLuint m_vao;
    int32_t m_vertCount;
    GLuint m_pos_vbo;
} MeshReference;

typedef struct MeshShaderReference{
    QOpenGLShader *m_vertex_shader;
    QOpenGLShader *m_frag_shader;
    QOpenGLShaderProgram *m_shader_prog;
} MeshShaderReference;

class Mesh
{
public:
    Mesh();
    ~Mesh();
    void setOpenGLContext(QOpenGLFunctions_4_0_Core *ctx) {gl = ctx;}

    // Mesh information getters
    int32_t getVAO() { return ref.m_vao; }
    int32_t getNumVerts() { return ref.m_vertCount; }
    MeshReference getMeshReference() { return ref; }


    // Utility mesh GL functions
    void bindVAO();
    void unbindVAO();
    void loadMesh();
    void loadShaders();
protected:
    // Mesh GL functions
    void createVAO();
    void pushToVAO();
private:
    // OpenGL context data
    MeshReference ref;
    QOpenGLFunctions_4_0_Core *gl;
    // OpenGL shaders
    MeshShaderReference shaders;
    // Mesh data
    std::vector<vertex> vertices;
};

#endif // MESH_H
