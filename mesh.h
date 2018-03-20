#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// VAO Indexes
#define VERTEX_VAO_IDX 0

// Uniform variable names and locations
#define MMAT_VARNAME "m_mat"
#define MMAT_IDX 0
#define VMAT_VARNAME "v_mat"
#define VMAT_IDX 1
#define PMAT_VARNAME "p_mat"
#define PMAT_IDX 2

#define MCOLOR_VARNAME "m_color"
#define MCOLOR_IDX 3
// -----------------
// FOR QT COMPATIBILITY
#include <QFile>
#include <QUrl>
std::string read_qrc_file(std::string qrc);
// -----------------

typedef struct _MeshReference{
    GLuint m_vao;
    int32_t m_vertCount;
    GLuint m_vert_vbo;
    GLuint m_idx_vbo;
} MeshReference;

typedef struct _MeshShaderReference{
    GLuint m_vertex_shader;
    GLuint m_frag_shader;
    GLuint m_shader_prog;
} MeshShaderReference;

typedef struct _MeshFace{
    glm::vec3 p[3];
} MeshFace;

template <typename T>
class Property{
public:
    Property(std::string n){
        name = n;
        dirty = false;
    }
    Property(T &p, std::string n){
        prop = p;
        name = n;
        dirty = true;
    }
    ~Property(){}

    void setUniformLocation(GLuint loc){
        location = loc;
    }
    void setProperty(T p){
        prop = p;
        dirty = true;
    }
    T& getProperty() { return prop; }
    bool isDirty() { return dirty; }
    void setDirty() { dirty = true; }
    void update(){
        if(!dirty) return;
        loadUniform(location, prop);
        dirty = false;
    }
protected:
    // Uniform Utility functions
    void loadUniform(GLuint loc, bool val);
    void loadUniform(GLuint loc, float val);
    void loadUniform(GLuint loc, glm::vec3 val);
    void loadUniform(GLuint loc, glm::mat4 val);
private:
    T prop;
    std::string name;
    GLuint location;
    bool dirty;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    void Cube();

    // Mesh information getters
    int32_t getVAO() { return ref.m_vao; }
    int32_t getNumVerts() { return ref.m_vertCount; }
    MeshReference getMeshReference() { return ref; }

    // Utility mesh GL functions
    void setDirty();
    void setModelMatrix(glm::mat4 mat);
    void setColor(glm::vec3 col);
    void printData();
    GLuint getViewMatrixLocation() { return viewMatrixLoc; }
    GLuint getProjMatrixLocation() { return projMatrixLoc; }
protected:
    friend class Model;
    friend class OBJLoader;

    // Mesh GL functions
    void createVAO();
    void pushToVAO();
    void updateVBO();
    void bindVAO();
    void unbindVAO();
    void cleanVAO();

    void loadMesh();
    bool loadShaders();

    void preRender();
    void renderMe();
    void postRender();

    bool createSHADER(std::string shader_code, GLenum type, GLuint &id);
    void attachSHADERPROG();
    void detachSHADERPROG();
    void cleanSHADERPROG();

    // Mesh data
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<MeshFace> faces;
private:
    // OpenGL context data
    MeshReference ref;
    // OpenGL shaders
    MeshShaderReference shaders;
    // Shader Mesh Data
    Property<glm::mat4> model_matrix;
    Property<glm::vec3> color;
    GLuint viewMatrixLoc, projMatrixLoc;
};

#endif // MESH_H
