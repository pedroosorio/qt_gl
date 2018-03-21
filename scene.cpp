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

void Scene::translateCameraBy(glm::vec3 t)
{
    camera.setPosition(camera.getPosition()+t);
}

void Scene::setCamerasAspectRatio(float as)
{
    camera.setAspectRatio(as);
}
