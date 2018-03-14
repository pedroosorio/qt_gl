#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "mesh.h"

class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine();
    void setOpenGLContext(QOpenGLFunctions_4_0_Core *ctx) {gl = ctx;}
    void init();

    void attachMesh(Mesh *m);
    void renderMeshes();
protected:
    void renderMesh(Mesh *mesh);
    void pre_render();
private:
    std::vector<Mesh *> meshes;
    QOpenGLFunctions_4_0_Core *gl;
};

#endif // RENDERENGINE_H
