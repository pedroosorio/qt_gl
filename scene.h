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
    void init();

    void attachModel(Model *model);
    void renderScene();

    Model *getModel(int idx);
    Camera *getCamera();

    void setCamerasAspectRatio(float as);

    // Temporary Function, will be replaced by a CameraController class
    void translateCameraBy(glm::vec3 t);
private:
    RenderEngine engine;
    std::vector<Model *> models;
    Camera camera;
};

#endif // SCENE_H
