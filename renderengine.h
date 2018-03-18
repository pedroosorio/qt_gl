#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QDebug>
#include <QOpenGLFunctions_4_0_Core>
#include "model.h"

class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine();

    // Render Engine setup functions
    void renderModels(std::vector<Model *> &models);
protected:
    // Render functions
    void render_model(Model *model);
    void pre_render();
};

#endif // RENDERENGINE_H
