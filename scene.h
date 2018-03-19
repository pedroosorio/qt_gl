#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include <vector>
#include "model.h"
#include "camera.h"
#include "renderengine.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void attachModel(Model *model);
    void renderScene();
    void init();
    Model *getModel(int idx);
    Camera *getCamera();
    void translateCameraBy(glm::vec3 t);
    void setCamerasAspectRatio(float as);
private:
    RenderEngine engine;
    std::vector<Model *> models;
    Camera camera;
};

#endif // SCENE_H
