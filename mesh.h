#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <QOpenGLFunctions_4_0_Core>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// VAO Indexes
#define POS_VAO_IDX 0

// Uniform variable names and locations
#define MMAT_VARNAME "m_mat"
#define MMAT_IDX 0
#define VMAT_VARNAME "v_mat"
#define VMAT_IDX 1
#define PMAT_VARNAME "p_mat"
#define PMAT_IDX 2

// -----------------
// FOR QT COMPATIBILITY
#include <QFile>
#include <QUrl>
std::string read_qrc_file(std::string qrc);
// -----------------

typedef struct MeshReference{
    GLuint m_vao;
    int32_t m_vertCount;
    GLuint m_pos_vbo;
} MeshReference;

typedef struct MeshShaderReference{
    GLuint m_vertex_shader;
    GLuint m_frag_shader;
    GLuint m_shader_prog;
} MeshShaderReference;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    // Mesh information getters
    int32_t getVAO() { return ref.m_vao; }
    int32_t getNumVerts() { return ref.m_vertCount; }
    MeshReference getMeshReference() { return ref; }

    // Utility mesh GL functions
    void setModelMatrix(glm::mat4 mat);
protected:
    friend class Model;

    // Mesh GL functions
    void createVAO();
    void pushToVAO();
    void bindVAO();
    void unbindVAO();
    void cleanVAO();
    void loadMesh();
    bool loadShaders();
    void renderMe();

    bool createSHADER(std::string shader_code, GLenum type, GLuint &id);
    void attachSHADERPROG();
    void detachSHADERPROG();
    void cleanSHADERPROG();

    // Uniform Utility functions
    void loadUniform(GLuint loc, bool val);
    void loadUniform(GLuint loc, float val);
    void loadUniform(GLuint loc, glm::vec3 val);
    void loadUniform(GLuint loc, glm::mat4 val);

    // Static uniform locations
    GLuint mmat_loc;

    // Aux
    bool model_matrix_dirty;
private:
    // OpenGL context data
    MeshReference ref;
    // OpenGL shaders
    MeshShaderReference shaders;
    // Mesh data
    std::vector<glm::vec3> vertices;
    glm::mat4 model_matrix;
};



#endif // MESH_H
