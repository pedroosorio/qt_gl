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
    void attachMesh(Mesh *m);
    void renderMeshes();
protected:
    // Render functions
    void render_mesh(Mesh *mesh);
    void pre_render();
    void upload_camera_mat();
private:
    // Render engine GL context data
    QOpenGLFunctions_4_0_Core *gl;
    // Render engine mesh list
    std::vector<Mesh *> meshes;
};

#endif // RENDERENGINE_H
