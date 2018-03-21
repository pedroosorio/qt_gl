#ifndef MODEL_H
#define MODEL_H

#include "objloader.h"
#include "mesh.h"
#include "property.h"

typedef struct _BoundingBox{
    glm::vec2 tb;
    glm::vec2 rl;
    glm::vec2 fb;
} BoundingBox;

class Model
{

public:
    Model(std::string obj_file_path);
    ~Model();

    //TODO:Remove this
    Mesh *getMesh() {return mesh; }

    void translateBy(glm::vec3 dt);
    void rotateBy(glm::vec3 dr);
    void scaleBy(glm::vec3 ds);

    void setModelPosition(glm::vec3 new_pos);
    glm::vec3 getModelPosition();
    void setModelRotation(glm::vec3 new_rot);
    glm::vec3 getModelRotation();
    void setModelScale(glm::vec3 new_scale);
    glm::vec3 getModelScale();
    void setColor(glm::vec3 col);
    void setUniformLocations(GLuint mmat_loc, GLuint col_loc);

    void updateModelMatrix();
    void render();
    bool init();
    void initBoundingBox();
protected:
    void Common();
private:
    // OpenGL context data
    std::string mesh_path;
    Mesh *mesh;
    // Model data
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Property<glm::mat4> model_matrix;
    Property<glm::vec3> color;
    BoundingBox bbox;
};

#endif // MODEL_H
