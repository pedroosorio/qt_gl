#include "model.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

Model::Model(std::string obj_file_path)
{
    mesh = new Mesh();
    OBJLoader loader(mesh);
    qDebug() << "Loaded OBJ: " << loader.LoadOBJ(obj_file_path);
    //mesh->Cube();
    Common();
}

Model::~Model()
{
    delete mesh;
}

void Model::Common()
{
    position = glm::vec3(0.0, 0.0, 0.0);
    rotation = glm::vec3(0.0, 0.0, 0.0);
    scale = glm::vec3(1.0, 1.0, 1.0);
    updateModel();
}

// ---------------------------------------------------

void Model::translateBy(glm::vec3 dt)
{
    position += dt;
    setDirtyMatrix();
}

void Model::rotateBy(glm::vec3 dr)
{
    rotation += dr;
    setDirtyMatrix();
}

void Model::scaleBy(glm::vec3 ds)
{
    scale += ds;
    setDirtyMatrix();
}

// ---------------------------------------------------

void Model::setModelPosition(glm::vec3 new_pos) {
    position = new_pos;
    setDirtyMatrix();
}

glm::vec3 Model::getModelPosition()
{
    return position;
}

void Model::setModelRotation(glm::vec3 new_rot)
{
    position = new_rot;
    setDirtyMatrix();
}

glm::vec3 Model::getModelRotation()
{
    return rotation;
}

void Model::setModelScale(glm::vec3 new_scale)
{
    scale = new_scale;
    setDirtyMatrix();
}

glm::vec3 Model::getModelScale()
{
    return scale;
}

// ---------------------------------------------------

void Model::updateModel()
{
    if(!mesh) return;
    mesh->setModelMatrix(glm::translate(glm::mat4(), position) *
                         glm::yawPitchRoll(rotation.x, rotation.y, rotation.z)*
                         glm::scale(glm::mat4(),scale));
    dirty_matrix = false;
}

void Model::render(Camera *camera)
{
    if(!mesh) return;
    if(dirty_matrix) {
        mesh->setModelMatrix(glm::translate(glm::mat4(), position) *
                             glm::yawPitchRoll(rotation.x, rotation.y, rotation.z)*
                             glm::scale(glm::mat4(),scale));
    }
    dirty_matrix = false;
    mesh->preRender();
    // Do something with the current mesh shaders
    camera->setViewMatrixUniformLocation(mesh->getViewMatrixLocation());
    camera->setProjMatrixUniformLocation(mesh->getProjMatrixLocation());
    camera->uploadMatricesToShader();
    mesh->renderMe();
    mesh->postRender();
}

bool Model::init()
{
    if(!mesh) return false;
    mesh->loadMesh();
    if(!mesh->loadShaders()) return false;
    return true;
}

void Model::setColor(glm::vec3 color)
{
    mesh->setColor(color);
}

void Model::setDirtyMatrix()
{
    dirty_matrix = true;
}
