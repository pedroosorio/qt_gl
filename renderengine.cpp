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
    camera_to_render = cam;
    for(auto model: models){
        model->rotateBy(glm::vec3(0.1, 0.0, 0.0));
        render_model(model);
    }
}

void RenderEngine::pre_render()
{
    GLctx->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLctx->glClearColor(1.0f, 0.3f, 0.3f, 0.5f);
}

void RenderEngine::render_model(Model *model)
{
    model->render(camera_to_render);
}
