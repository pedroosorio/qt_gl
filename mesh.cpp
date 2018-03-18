#include "mesh.h"
#include "qopengl.h"

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

Mesh::Mesh() : model_matrix("model_matrix"), color("color")
{
    //setup mesh gl
    ref.m_vao = ref.m_vertCount = ref.m_vert_vbo = 0;
    shaders.m_frag_shader = shaders.m_vertex_shader = shaders.m_shader_prog = 0;

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

    ref.m_vertCount = static_cast<int32_t>(vertices.size());

    model_matrix.setProperty(glm::translate(glm::mat4(), glm::vec3(0.0f , 0.0f , 0.0f)) *
                            glm::yawPitchRoll(0.0f, 0.0f, 0.0f)*
                            glm::scale(glm::mat4(), glm::vec3(1.0f , 1.0f , 1.0f)));
    color.setProperty(glm::vec3(0.5, 0.5, 0.5));
}

Mesh::~Mesh()
{
    cleanVAO();
    cleanSHADERPROG();
}

// ---------------------------------------------------

void Mesh::setDirty()
{
    model_matrix.setDirty();
    color.setDirty();
}

void Mesh::setModelMatrix(glm::mat4 mat)
{
    model_matrix.setProperty(mat);
}

void Mesh::setColor(glm::vec3 col)
{
    color.setProperty(col);
}

// ---------------------------------------------------

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
    GLctx->glBindAttribLocation(shaders.m_shader_prog, BCOLOR_IDX, BCOLOR_VARNAME);

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
    color.setUniformLocation(GLctx->glGetUniformLocation(shaders.m_shader_prog, BCOLOR_VARNAME));
    return true;
}

void Mesh::renderMe()
{
    bindVAO();
    GLctx->glEnableVertexAttribArray(VERTEX_VAO_IDX);
    attachSHADERPROG();

    model_matrix.update();
    color.update();

    if(indices.size()){
        GLctx->glDrawElements( GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, (void*)0);
    } else GLctx->glDrawArrays(GL_TRIANGLES, 0, ref.m_vertCount);

    GLctx->glDisableVertexAttribArray(VERTEX_VAO_IDX);
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
    if(vertices.size() > 0){
        GLctx->glGenBuffers(1, &ref.m_vert_vbo);
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, ref.m_vert_vbo);
        GLctx->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        GLctx->glVertexAttribPointer(VERTEX_VAO_IDX, 3, GL_FLOAT, GL_FALSE, 0, 0);
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(indices.size() > 0){
        GLctx->glGenBuffers(1, &ref.m_idx_vbo);
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref.m_idx_vbo);
        GLctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::updateVBO()
{
    if(vertices.size() > 0){
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, ref.m_vert_vbo);
        GLctx->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        GLctx->glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(indices.size() > 0){
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref.m_idx_vbo);
        GLctx->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::bindVAO()
{
    GLctx->glBindVertexArray(ref.m_vao);
    if(indices.size() > 0) GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ref.m_idx_vbo);
}

void Mesh::unbindVAO()
{
    GLctx->glBindVertexArray(0);
    if(indices.size() > 0) GLctx->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::cleanVAO()
{
    unbindVAO();
    GLctx->glDeleteVertexArrays(1, &ref.m_vao);
    GLctx->glDeleteBuffers(1, &ref.m_vert_vbo);
    GLctx->glDeleteBuffers(1, &ref.m_idx_vbo);
}

// ---------------------------------------------------

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

// ---------------------------------------------------

template<typename T>
void MeshProperty<T>::loadUniform(GLuint loc, bool val)
{
    GLctx->glUniform1f(loc, val*1.0f);
}
template<typename T>
void MeshProperty<T>::loadUniform(GLuint loc, float val)
{
    GLctx->glUniform1f(loc, val);
}
template<typename T>
void MeshProperty<T>::loadUniform(GLuint loc, glm::vec3 val)
{
    GLctx->glUniform3f(loc, val.x, val.y, val.z);
}
template<typename T>
void MeshProperty<T>::loadUniform(GLuint loc, glm::mat4 val)
{
    GLctx->glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
}

// ---------------------------------------------------

