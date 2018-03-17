#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "mesh.h"
#include <QDebug>

class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine();
    void setOpenGLContext(QOpenGLFunctions_4_0_Core *ctx) {gl = ctx;}

    // Render Engine setup functions
    void init();
    void attachModel(Model *model);
    void renderModels();
protected:
    // Render functions
    void render_model(Model *model);
    void pre_render();
    void upload_camera_mat();
private:
    // Render engine GL context data
    QOpenGLFunctions_4_0_Core *gl;
    // Render engine model list
    std::vector<Model *> models;
};

#endif // RENDERENGINE_H
