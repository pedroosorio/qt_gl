#include "mesh.h"
#include "qopengl.h"
#include <QDebug>

extern QOpenGLFunctions_4_0_Core *GLctx;

#define STANDARD_VERT_SHADER "://Shaders/vertex_shader.vert"
#define STANDARD_FRAG_SHADER "://Shaders/fragment_shader.frag"

std::string read_qrc_file(std::string qrc){
    QFile file(qrc.c_str());
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error reading qrc : " << file.errorString();
    }
    QTextStream in(&file);
    return in.readAll().toStdString();
}

Mesh::Mesh()
{
    //setup mesh gl
    ref.m_vao = ref.m_vertCount = ref.m_pos_vbo = 0;
    shaders.m_frag_shader = shaders.m_vertex_shader = shaders.m_shader_prog = 0;
    model_matrix_dirty = true;

    //simple quad mesh
    vertices.push_back(glm::vec3(-0.5, -0.5, 0.0));
    vertices.push_back(glm::vec3(0.5, 0.5, 0.0));
    vertices.push_back(glm::vec3(-0.5, 0.5, 0.0));
    vertices.push_back(glm::vec3(-0.5, -0.5, 0.0));
    vertices.push_back(glm::vec3(0.5, -0.5, 0.0));
    vertices.push_back(glm::vec3(0.5, 0.5, 0.0));
    ref.m_vertCount = static_cast<int32_t>(vertices.size());

    model_matrix = glm::translate(glm::mat4(), glm::vec3(0.0f , 0.0f , 0.0f)) *
                   glm::yawPitchRoll(0.0f, 0.0f, 0.0f)*
                   glm::scale(glm::mat4(), glm::vec3(1.0f , 1.0f , 1.0f));
}

Mesh::~Mesh()
{
    cleanVAO();
    cleanSHADERPROG();
}

void Mesh::setModelMatrix(glm::mat4 mat)
{
    model_matrix = mat;
    model_matrix_dirty = true;
}

void Mesh::loadMesh()
{
    createVAO();
    bindVAO();
    pushToVAO();
    unbindVAO();
}

bool Mesh::loadShaders()
{
    if(!createSHADER(read_qrc_file(STANDARD_VERT_SHADER), GL_VERTEX_SHADER, shaders.m_vertex_shader)) return false;
    if(!createSHADER(read_qrc_file(STANDARD_FRAG_SHADER), GL_FRAGMENT_SHADER, shaders.m_frag_shader)) return false;
    shaders.m_shader_prog = GLctx->glCreateProgram();
    GLctx->glAttachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    GLctx->glAttachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    GLctx->glBindAttribLocation(shaders.m_shader_prog, MMAT_IDX, MMAT_VARNAME);

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
    mmat_loc = GLctx->glGetUniformLocation(shaders.m_shader_prog, MMAT_VARNAME);
    return true;
}

void Mesh::renderMe()
{
    bindVAO();
    GLctx->glEnableVertexAttribArray(POS_VAO_IDX);
    attachSHADERPROG();
    // Update model matrix
    if(model_matrix_dirty) {
        loadUniform(mmat_loc, model_matrix);
        model_matrix_dirty = false;
    }
    GLctx->glDrawArrays(GL_TRIANGLES, 0, getNumVerts());
    GLctx->glDisableVertexAttribArray(POS_VAO_IDX);
    detachSHADERPROG();
    unbindVAO();
}

// ---------------------------------------------------------------

void Mesh::createVAO()
{
    GLctx->glGenVertexArrays(1, &ref.m_vao);
}

void Mesh::pushToVAO()
{
    GLctx->glGenBuffers(1, &ref.m_pos_vbo);
    GLctx->glBindBuffer(GL_ARRAY_BUFFER, ref.m_pos_vbo);
    GLctx->glBufferData(GL_ARRAY_BUFFER, ref.m_vertCount * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
    GLctx->glVertexAttribPointer(POS_VAO_IDX, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::bindVAO()
{
    GLctx->glBindVertexArray(ref.m_vao);
}

void Mesh::unbindVAO()
{
    GLctx->glBindVertexArray(0);
}

void Mesh::cleanVAO()
{
    unbindVAO();
    GLctx->glDeleteVertexArrays(1, &ref.m_vao);
    GLctx->glDeleteBuffers(1, &ref.m_pos_vbo);
}

bool Mesh::createSHADER(std::string shader_code, GLenum type, GLuint &id)
{
    shader_code += '\0';
    const char *code = shader_code.c_str();
    id = GLctx->glCreateShader(type);
    GLctx->glShaderSource(id, 1, &code, NULL);
    GLctx->glCompileShader(id);
    GLint compile_status = GL_FALSE;
    GLctx->glGetShaderiv(id, GL_COMPILE_STATUS , &compile_status);

    // Debug purposes only
    if(compile_status == GL_FALSE){
        GLint maxLength = 0;
        GLctx->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        GLctx->glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        qDebug() << "Error compiling shader" << id << ":" << std::string(errorLog.begin(),errorLog.end()).c_str();
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        GLctx->glDeleteShader(id); // Don't leak the shader.
        return false;
    }

    return true;
}

void Mesh::attachSHADERPROG()
{
    GLctx->glUseProgram(shaders.m_shader_prog);
}

void Mesh::detachSHADERPROG()
{
    GLctx->glUseProgram(0);
}

void Mesh::cleanSHADERPROG()
{
    detachSHADERPROG();
    GLctx->glDetachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    GLctx->glDetachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    GLctx->glDeleteShader(shaders.m_vertex_shader);
    GLctx->glDeleteShader(shaders.m_frag_shader);
    GLctx->glDeleteProgram(shaders.m_shader_prog);
}


void Mesh::loadUniform(GLuint loc, bool val)
{
    GLctx->glUniform1f(loc, val*1.0f);
}

void Mesh::loadUniform(GLuint loc, float val)
{
    GLctx->glUniform1f(loc, val);
}

void Mesh::loadUniform(GLuint loc, glm::vec3 val)
{
    GLctx->glUniform3f(loc, val.x, val.y, val.z);
}

void Mesh::loadUniform(GLuint loc, glm::mat4 val)
{
    GLctx->glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
}



