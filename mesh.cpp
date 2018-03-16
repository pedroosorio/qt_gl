#include "mesh.h"
#include "qopengl.h"
#include <QDebug>

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

    //simple quad mesh
    vertices.push_back(vertex(-0.5, -0.5, 0.0));
    vertices.push_back(vertex(0.5, 0.5, 0.0));
    vertices.push_back(vertex(-0.5, 0.5, 0.0));
    vertices.push_back(vertex(-0.5, -0.5, 0.0));
    vertices.push_back(vertex(0.5, -0.5, 0.0));
    vertices.push_back(vertex(0.5, 0.5, 0.0));
    ref.m_vertCount = static_cast<int32_t>(vertices.size());
}

Mesh::~Mesh()
{
    cleanVAO();
    cleanSHADERPROG();
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
    shaders.m_shader_prog = gl->glCreateProgram();
    gl->glAttachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    gl->glAttachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    gl->glLinkProgram(shaders.m_shader_prog);
    GLint isLinked = 0;
    gl->glGetProgramiv(shaders.m_shader_prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        gl->glGetProgramiv(shaders.m_shader_prog, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        gl->glGetProgramInfoLog(shaders.m_shader_prog, maxLength, &maxLength, &infoLog[0]);
        qDebug() << "Error linking program" << shaders.m_shader_prog << ":" << std::string(infoLog.begin(),infoLog.end()).c_str();
        cleanSHADERPROG();
        return false;
    }
    gl->glDetachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    gl->glDetachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    return true;
}

void Mesh::renderMe()
{
    bindVAO();
    gl->glEnableVertexAttribArray(POS_VAO_IDX);
    attachSHADERPROG();
    gl->glDrawArrays(GL_TRIANGLES, 0, getNumVerts());
    gl->glDisableVertexAttribArray(POS_VAO_IDX);
    detachSHADERPROG();
    unbindVAO();
}

// ---------------------------------------------------------------

void Mesh::createVAO()
{
    gl->glGenVertexArrays(1, &ref.m_vao);
}

void Mesh::pushToVAO()
{
    gl->glGenBuffers(1, &ref.m_pos_vbo);
    gl->glBindBuffer(GL_ARRAY_BUFFER, ref.m_pos_vbo);
    gl->glBufferData(GL_ARRAY_BUFFER, ref.m_vertCount * sizeof(vertex), &vertices[0], GL_DYNAMIC_DRAW);
    gl->glVertexAttribPointer(POS_VAO_IDX, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::bindVAO()
{
    gl->glBindVertexArray(ref.m_vao);
}

void Mesh::unbindVAO()
{
    gl->glBindVertexArray(0);
}

void Mesh::cleanVAO()
{
    unbindVAO();
    gl->glDeleteVertexArrays(1, &ref.m_vao);
    gl->glDeleteBuffers(1, &ref.m_pos_vbo);
}

bool Mesh::createSHADER(std::string shader_code, GLenum type, GLuint &id)
{
    shader_code += '\0';
    const char *code = shader_code.c_str();
    id = gl->glCreateShader(type);
    gl->glShaderSource(id, 1, &code, NULL);
    gl->glCompileShader(id);
    GLint compile_status = GL_FALSE;
    gl->glGetShaderiv(id, GL_COMPILE_STATUS , &compile_status);

    // Debug purposes only
    if(compile_status == GL_FALSE){
        GLint maxLength = 0;
        gl->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        gl->glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
        qDebug() << "Error compiling shader" << id << ":" << std::string(errorLog.begin(),errorLog.end()).c_str();
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        gl->glDeleteShader(id); // Don't leak the shader.
        return false;
    }

    return true;
}

void Mesh::attachSHADERPROG()
{
    gl->glUseProgram(shaders.m_shader_prog);
}

void Mesh::detachSHADERPROG()
{
    gl->glUseProgram(0);
}

void Mesh::cleanSHADERPROG()
{
    detachSHADERPROG();
    gl->glDetachShader(shaders.m_shader_prog, shaders.m_vertex_shader);
    gl->glDetachShader(shaders.m_shader_prog, shaders.m_frag_shader);
    gl->glDeleteShader(shaders.m_vertex_shader);
    gl->glDeleteShader(shaders.m_frag_shader);
    gl->glDeleteProgram(shaders.m_shader_prog);
}


