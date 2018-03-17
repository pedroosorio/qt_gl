#ifndef MESH_H
#define MESH_H

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "ThirdParty/glm/gtc/quaternion.hpp"
#include "ThirdParty/glm/gtx/quaternion.hpp"
#include "ThirdParty/glm/gtx/euler_angles.hpp"
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
    GLuint m_vertex_shader;
    GLuint m_frag_shader;
    GLuint m_shader_prog;
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
    void setModelMatrix(glm::mat4 mat) { model_matrix = mat; model_matrix_dirty = true; }
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
    QOpenGLFunctions_4_0_Core *gl;
    // OpenGL shaders
    MeshShaderReference shaders;
    // Mesh data
    std::vector<vertex> vertices;
    glm::mat4 model_matrix;
};


class Model
{

public:
    Model(std::vector<vertex> vertex_data);
    Model(std::string obj_file_path);
    ~Model();
    void setOpenGLContext(QOpenGLFunctions_4_0_Core *ctx) {gl = ctx; if(mesh) mesh->setOpenGLContext(ctx);}

    void translateBy(pos translation) { position += translation; setDirty(); }
    void rotateBy(rot rots) { rotation += rots; setDirty(); }
    void scaleBy(glm::vec3 scals) { scale += scals; setDirty(); }

    void setModelPosition(pos new_pos) { position = new_pos; setDirty(); }
    pos getModelPosition() { return position; }
    void setModelRotation(rot new_rot) { position = new_rot; setDirty(); }
    rot getModelRotation() { return rotation; }
    void setModelScale(glm::vec3 new_scale) { scale = new_scale; setDirty(); }
    pos getModelScale() { return scale; }

    void updateModel(){
        if(!mesh) return;
        setDirty();
    }

    void render() {
        if(!mesh) return;
        if(dirty) mesh->setModelMatrix(glm::translate(glm::mat4(), position) *
                                       glm::yawPitchRoll(rotation.x, rotation.y, rotation.z)*
                                       glm::scale(glm::mat4(),scale));
        dirty = false;
        mesh->renderMe();
    }

    bool init() {
        if(!mesh) return false;
        mesh->loadMesh();
        if(!mesh->loadShaders()) return false;
        return true;
    }

protected:
    void Common();
    void setDirty() { mesh->model_matrix_dirty = dirty = true; }
private:
    // OpenGL context data
    Mesh *mesh;
    QOpenGLFunctions_4_0_Core *gl;
    // Model data
    pos position;
    rot rotation;
    glm::vec3 scale;
    bool dirty;
};
#endif // MESH_H
