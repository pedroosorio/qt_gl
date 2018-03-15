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
        mesh->loadShaders();
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
        renderMesh(mesh);
    }
}

void RenderEngine::pre_render()
{
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glClearColor(1.0f, 0.3f, 0.0f, 0.0f);
}

void RenderEngine::renderMesh(Mesh *mesh)
{
    mesh->bindVAO();
    gl->glEnableVertexAttribArray(POS_VAO_IDX);
    gl->glDrawArrays(GL_TRIANGLES, 0, mesh->getNumVerts());
    gl->glDisableVertexAttribArray(POS_VAO_IDX);
    mesh->unbindVAO();
}
