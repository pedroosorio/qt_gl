#include "model.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

Model::Model(std::string obj_file_path) :
    // Init shader-uploadable properties with its shader-variable name
    model_matrix("model_matrix"), color("model_color")
{
    mesh = new Mesh();
    mesh_path = obj_file_path;
}

Model::~Model()
{
    //TODO: Move this responsability to the render engine, to get rid of the mesh because
    // multiple models can share the same mesh
    delete mesh;
}

void Model::Common()
{
    initBoundingBox(); //Inits position and scale.
    //TODO: Remove scaling and reposition of object when not needed
    rotation = glm::vec3(0.0, 0.0, 0.0);
    color.setProperty(glm::vec3(0.3, 0.8, 0.2));
    updateModelMatrix();
}

// ---------------------------------------------------

void Model::translateBy(glm::vec3 dt)
{
    position += dt;
    model_matrix.setDirty();
}

void Model::rotateBy(glm::vec3 dr)
{
    rotation += dr;
    model_matrix.setDirty();
}

void Model::scaleBy(glm::vec3 ds)
{
    scale += ds;
    model_matrix.setDirty();
}

// ---------------------------------------------------

void Model::setModelPosition(glm::vec3 new_pos) {
    position = new_pos;
    model_matrix.setDirty();
}

glm::vec3 Model::getModelPosition()
{
    return position;
}

void Model::setModelRotation(glm::vec3 new_rot)
{
    position = new_rot;
    model_matrix.setDirty();
}

glm::vec3 Model::getModelRotation()
{
    return rotation;
}

void Model::setModelScale(glm::vec3 new_scale)
{
    scale = new_scale;
    model_matrix.setDirty();
}

glm::vec3 Model::getModelScale()
{
    return scale;
}

// ---------------------------------------------------

void Model::updateModelMatrix()
{
    if(!mesh) return;
    model_matrix.setProperty(glm::translate(glm::mat4(), position) *
                         glm::yawPitchRoll(rotation.x, rotation.y, rotation.z)*
                         glm::scale(glm::mat4(),scale));
}

void Model::render()
{
    if(!mesh) return;
    if(model_matrix.isDirty()) updateModelMatrix();
    // At this point, the mesh's reference shader is being used and active
    // Upload all the Model-specific properties to the shader
    model_matrix.update();
    color.update();
    mesh->renderMe();
}

bool Model::init()
{
    if(!mesh) return false;
    //TODO: Check if the mesh is cached before loading it from file or move this
    //elsewhere, perhaps the render engine that will own the meshes
    OBJLoader loader(mesh);
    qDebug() << "Loaded OBJ: " << loader.LoadOBJ(mesh_path);
    Common();
    // Now that the data is in the mesh, load it to the GPU
    mesh->loadMesh();
    return true;
}

void Model::initBoundingBox()
{
    bbox.tb.x = -1000000.0; bbox.tb.y = 1000000.0;
    bbox.rl.x = -1000000.0; bbox.rl.y = 1000000.0;
    bbox.tb.x = -1000000.0; bbox.tb.y = 1000000.0;
    for(auto vertex: mesh->vertices){
        if(vertex.x > bbox.rl.x) bbox.rl.x = vertex.x;
        if(vertex.x < bbox.rl.y) bbox.rl.y = vertex.x;
        if(vertex.y > bbox.tb.x) bbox.tb.x = vertex.y;
        if(vertex.y < bbox.tb.y) bbox.tb.y = vertex.y;
        if(vertex.z > bbox.fb.x) bbox.fb.x = vertex.z;
        if(vertex.z < bbox.fb.y) bbox.fb.y = vertex.z;
    }

    // Make the model unit-normal
    float width = sqrt((bbox.rl.x-bbox.rl.y)*(bbox.rl.x-bbox.rl.y));
    float height = sqrt((bbox.tb.x-bbox.tb.y)*(bbox.tb.x-bbox.tb.y));
    float depth = sqrt((bbox.fb.x-bbox.fb.y)*(bbox.fb.x-bbox.fb.y));
    float multiplier = width;
    multiplier = glm::max(multiplier, height);
    multiplier = glm::max(multiplier, depth);
    multiplier = 1.0/multiplier;

    scale = glm::vec3(multiplier, multiplier, multiplier);
    position = glm::vec3(-bbox.rl.x+width/2.0,
                         -bbox.tb.x+height/2.0,
                         bbox.fb.x-depth/2.0) * multiplier;

}

void Model::setColor(glm::vec3 col)
{
    color.setProperty(col);
}

void Model::setUniformLocations(GLuint mmat_loc, GLuint col_loc)
{
    model_matrix.setUniformLocation(mmat_loc);
    color.setUniformLocation(col_loc);
}
