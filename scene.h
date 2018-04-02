#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include <vector>
#include "model.h"
#include "camera.h"
#include "renderengine.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>

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
public slots:
    // Event handlers
    void onKeyPress(QKeyEvent *event);
    void onKeyRelease(QKeyEvent *event);
    void onDoubleClick(QMouseEvent *event);
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);
private:
    RenderEngine engine;
    std::vector<Model *> models;
    Camera camera;
};

#endif // SCENE_H
