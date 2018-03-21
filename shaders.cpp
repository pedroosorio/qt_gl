#include "shaders.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

std::string read_qrc_file(std::string qrc){
    QFile file(qrc.c_str());
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error reading qrc : " << file.errorString();
    }
    QTextStream in(&file);
    return in.readAll().toStdString();
}

ShaderProgram::ShaderProgram(ShaderProgramType type)
{
    vs_id = fs_id = prog_id = 0;
    model_matrix_loc = view_matrix_loc = proj_matrix_loc = 0;
    s_type = type;

    switch(s_type){
        case NO_TEX_MODEL_SHADER:{
            vs_path = NO_TEX_MODEL_VS;
            fs_path = NO_TEX_MODEL_FS;
            break;
        }
        case TEX_MODEL_SHADER:
        default:{
            qDebug() << "Unsupported shader type.";
            break;
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    clean();
}

void ShaderProgram::attach()
{
    GLctx->glUseProgram(prog_id);
}

void ShaderProgram::detach()
{
    GLctx->glUseProgram(0);
}

void ShaderProgram::init()
{
    load();
}

bool ShaderProgram::load()
{
    if(!create(read_qrc_file(vs_path), GL_VERTEX_SHADER, vs_id)) return false;
    if(!create(read_qrc_file(fs_path), GL_FRAGMENT_SHADER, fs_id)) return false;
    prog_id = GLctx->glCreateProgram();
    GLctx->glAttachShader(prog_id, vs_id);
    GLctx->glAttachShader(prog_id, fs_id);

    //TODO: Convert this to a vector per shader type
    GLctx->glBindAttribLocation(prog_id, MMAT_IDX, MMAT_VARNAME);
    GLctx->glBindAttribLocation(prog_id, VMAT_IDX, VMAT_VARNAME);
    GLctx->glBindAttribLocation(prog_id, PMAT_IDX, PMAT_VARNAME);
    GLctx->glBindAttribLocation(prog_id, MCOLOR_IDX, MCOLOR_VARNAME);
    // Link and verify linkage
    GLctx->glLinkProgram(prog_id);
    GLint isLinked = 0;
    GLctx->glGetProgramiv(prog_id, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        GLctx->glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        GLctx->glGetProgramInfoLog(prog_id, maxLength, &maxLength, &infoLog[0]);
        qDebug() << "Error linking program" << prog_id << ":" << std::string(infoLog.begin(),infoLog.end()).c_str();
        clean();
        return false;
    }
    GLctx->glDetachShader(prog_id, vs_id);
    GLctx->glDetachShader(prog_id, fs_id);

    // Retrieve uniform locations
    model_matrix_loc = GLctx->glGetUniformLocation(prog_id, MMAT_VARNAME);
    view_matrix_loc = GLctx->glGetUniformLocation(prog_id, VMAT_VARNAME);
    proj_matrix_loc = GLctx->glGetUniformLocation(prog_id, PMAT_VARNAME);
    model_color_loc = GLctx->glGetUniformLocation(prog_id, MCOLOR_VARNAME);
    return true;
}

bool ShaderProgram::create(std::string shader_code, GLenum type, GLuint &id)
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

void ShaderProgram::clean()
{
    detach();
    GLctx->glDetachShader(prog_id, vs_id);
    GLctx->glDetachShader(prog_id, fs_id);
    GLctx->glDeleteShader(vs_id);
    GLctx->glDeleteShader(fs_id);
    GLctx->glDeleteProgram(prog_id);
}
