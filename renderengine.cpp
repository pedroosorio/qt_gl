#include "renderengine.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

RenderEngine::RenderEngine()
{

}

RenderEngine::~RenderEngine()
{

}

void RenderEngine::renderModels(std::vector<Model *>& models, Camera *cam)
{
    pre_render();

    //Order models list by Shader ID and VAO ID
    // 1 - Activate the shader program
    // *** Update View and Projection Matrices (Camera)
    // *** Update some Scene variables like ambient light
    // 2 - Activate the mesh's VAO
    // *** Update model-specific shader variables
    // *** Render the model

    for(auto model: models){
        model->getMesh()->bindVAO();
        shader.attach();

        cam->setViewMatrixUniformLocation(shader.getViewMatrixLocation());
        cam->setProjMatrixUniformLocation(shader.getProjectionMatrixLocation());
        cam->uploadMatricesToShader();

        model->setUniformLocations(shader.getModelMatrixLocation(), shader.getModelColorLocation());
        model->rotateBy(glm::vec3(0.03, 0.0, 0.0));
        model->render();

        shader.detach();
        model->getMesh()->unbindVAO();
    }
}

void RenderEngine::init()
{
    shader.init();
}

void RenderEngine::pre_render()
{
    GLctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLctx->glClearColor(1.0f, 0.3f, 0.3f, 0.5f);
}
