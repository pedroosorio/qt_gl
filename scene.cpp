#include "scene.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

Scene::Scene()
{

}

Scene::~Scene()
{
    for(auto model: models){
        delete model;
    }
}

void Scene::attachModel(Model *model)
{
    models.push_back(model);
}

void Scene::renderScene()
{
    engine.renderModels(models, &camera);
}

void Scene::init()
{
    engine.init();
    for(auto model: models){
        model->init();
    }
}

Model *Scene::getModel(int idx)
{
    if(idx < 0 || idx >= models.size()) return nullptr;
    return models[idx];
}

Camera *Scene::getCamera()
{
    return &camera;
}

void Scene::setCamerasAspectRatio(float as)
{
    camera.setAspectRatio(as);
}

void Scene::onKeyPress(QKeyEvent *event)
{
    return;
}

void Scene::onKeyRelease(QKeyEvent *event)
{
    return;
}

void Scene::onDoubleClick(QMouseEvent *event)
{
    return;
}

//Pass the important info to the camera controller, so he can
//implement the control of the camera, be it FPS, 3rd Person ...
void Scene::onMouseMove(QMouseEvent *event)
{

}

void Scene::onMousePress(QMouseEvent *event)
{

}

void Scene::onMouseRelease(QMouseEvent *event)
{

}
