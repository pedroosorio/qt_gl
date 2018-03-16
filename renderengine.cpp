#include "renderengine.h"

RenderEngine::RenderEngine()
{
    gl = nullptr;
}

RenderEngine::~RenderEngine()
{
    for(auto mesh: meshes){
        delete mesh;
    }
}

void RenderEngine::init()
{
    for(auto mesh: meshes){
        mesh->loadMesh();
        if(!mesh->loadShaders()){
            qDebug() << "Failed to load shaders for mesh.";
        }
    }
}

void RenderEngine::attachMesh(Mesh *m)
{
    meshes.push_back(m);
    m->setOpenGLContext(gl);
}

void RenderEngine::renderMeshes()
{
    pre_render();
    for(auto mesh: meshes){
        render_mesh(mesh);
    }
}

void RenderEngine::pre_render()
{
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
}

void RenderEngine::render_mesh(Mesh *mesh)
{
    mesh->renderMe();
}
