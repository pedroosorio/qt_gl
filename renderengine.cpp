#include "renderengine.h"

RenderEngine::RenderEngine()
{
    gl = nullptr;
}

RenderEngine::~RenderEngine()
{
    for(auto model: models){
        delete model;
    }
}

void RenderEngine::init()
{
    for(auto model: models){
        if(!model->init()) qDebug() << "Failed to load shaders for mesh.";
    }
}

void RenderEngine::attachModel(Model *model)
{
    models.push_back(model);
    model->setOpenGLContext(gl);
}

void RenderEngine::renderModels()
{
    pre_render();
    for(auto model: models){
        model->rotateBy(glm::vec3(0.01f, 0.0, 0.0));
        render_model(model);
    }
}

void RenderEngine::pre_render()
{
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glClearColor(1.0f, 0.3f, 0.3f, 0.5f);
}

void RenderEngine::render_model(Model *model)
{
    model->render();
}
