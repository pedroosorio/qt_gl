#ifndef SHADERS_H
#define SHADERS_H

#include "glm/glm.hpp"
#include <QOpenGLFunctions_4_0_Core>
#include <vector>
#include <QDebug>

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
#define NO_TEX_MODEL_VS "://Shaders/vertex_shader.vert"
#define NO_TEX_MODEL_FS "://Shaders/fragment_shader.frag"

#include <QFile>
#include <QUrl>
std::string read_qrc_file(std::string qrc);
// -----------------

typedef enum _ShaderProgramType{
    NO_TEX_MODEL_SHADER = 0,
    TEX_MODEL_SHADER
} ShaderProgramType;
class ShaderManager;

class ShaderProgram
{
public:
    ShaderProgram(ShaderProgramType type = NO_TEX_MODEL_SHADER);
    ~ShaderProgram();

    inline GLuint getProgram() { return prog_id; }
    inline GLuint getModelMatrixLocation() { return model_matrix_loc; }
    inline GLuint getViewMatrixLocation() { return view_matrix_loc; }
    inline GLuint getProjectionMatrixLocation() { return proj_matrix_loc; }
    inline GLuint getModelColorLocation() { return model_color_loc; }

    void attach();
    void detach();
    void init();
protected:
    friend class ShaderManager;
    bool load();
    bool create(std::string shader_code, GLenum type, GLuint &id);
    void clean();
private:
    // Shader IDs
    GLuint vs_id;
    GLuint fs_id;
    GLuint prog_id;
    ShaderProgramType s_type;
    // Shader file paths
    std::string vs_path;
    std::string fs_path;
    // Shader Uniform Variables
    GLuint model_matrix_loc;
    GLuint view_matrix_loc;
    GLuint proj_matrix_loc;
    GLuint model_color_loc;
};

class ShaderManager
{
public:
    static ShaderManager& getInstance()
    {
        static ShaderManager instance;
        return instance;
    }
    int32_t getShaderCount() { return static_cast<int32_t>(shaders.size()); }

    ShaderProgram* getShader(int idx){
        if(idx < 0 || idx >= getShaderCount()) return nullptr;
        return &shaders[idx];
    }
    void addShader(ShaderProgram& shader){
        shaders.push_back(shader);
    }
private:
    ShaderManager() {}                    // Constructor? (the {} brackets) are needed here.
    std::vector<ShaderProgram> shaders;
public:
    ShaderManager(ShaderManager const&) = delete;
    void operator=(ShaderManager const&)  = delete;
};

#endif // SHADERS_H

/*

bool Mesh::loadShaders()
{
    if(!createSHADER(read_qrc_file(STANDARD_VERT_SHADER), GL_VERTEX_SHADER, shaders.m_vertex_shader)) return false;
    if(!createSHADER(read_qrc_file(STANDARD_FRAG_SHADER), GL_FRAGMENT_SHADER, shaders.m_frag_shader)) return false;
    shaders.m_shader_prog = GLctx->glCreateProgram();
    GLctx->glAttachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    GLctx->glAttachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    GLctx->glBindAttribLocation(shaders.m_shader_prog, MMAT_IDX, MMAT_VARNAME);
    GLctx->glBindAttribLocation(shaders.m_shader_prog, MCOLOR_IDX, MCOLOR_VARNAME);
    GLctx->glBindAttribLocation(shaders.m_shader_prog, VMAT_IDX, VMAT_VARNAME);
    GLctx->glBindAttribLocation(shaders.m_shader_prog, PMAT_IDX, PMAT_VARNAME);
    // Link and verify linkage
    GLctx->glLinkProgram(shaders.m_shader_prog);
    GLint isLinked = 0;
    GLctx->glGetProgramiv(shaders.m_shader_prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        GLctx->glGetProgramiv(shaders.m_shader_prog, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        GLctx->glGetProgramInfoLog(shaders.m_shader_prog, maxLength, &maxLength, &infoLog[0]);
        qDebug() << "Error linking program" << shaders.m_shader_prog << ":" << std::string(infoLog.begin(),infoLog.end()).c_str();
        cleanSHADERPROG();
        return false;
    }
    GLctx->glDetachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    GLctx->glDetachShader(shaders.m_shader_prog, shaders.m_frag_shader);

    // Retrieve final location of model matrix uniform
    model_matrix.setUniformLocation(GLctx->glGetUniformLocation(shaders.m_shader_prog, MMAT_VARNAME));
    color.setUniformLocation(GLctx->glGetUniformLocation(shaders.m_shader_prog, MCOLOR_VARNAME));
    viewMatrixLoc = GLctx->glGetUniformLocation(shaders.m_shader_prog, VMAT_VARNAME);
    projMatrixLoc = GLctx->glGetUniformLocation(shaders.m_shader_prog, PMAT_VARNAME);
    return true;
}
void Mesh::preRender()
{
    bindVAO();
    GLctx->glEnableVertexAttribArray(VERTEX_VAO_IDX);
    attachSHADERPROG();

    model_matrix.update();
    color.update();
}

void Mesh::renderMe()
{
    if(indices.size()){
        GLctx->glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, (void*)0);
    } else GLctx->glDrawArrays(GL_TRIANGLES, 0, ref.m_vertCount);
}

void Mesh::postRender()
{
    GLctx->glDisableVertexAttribArray(VERTEX_VAO_IDX);
    detachSHADERPROG();
    unbindVAO();
}

*/
