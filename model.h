#ifndef MODEL_H
#define MODEL_H

#include "objloader.h"
#include "camera.h"
#include "mesh.h"

class Model
{

public:
    Model(std::vector<glm::vec3> vertex_data);
    Model(std::string obj_file_path);
    ~Model();

    void translateBy(glm::vec3 dt);
    void rotateBy(glm::vec3 dr);
    void scaleBy(glm::vec3 ds);

    void setModelPosition(glm::vec3 new_pos);
    glm::vec3 getModelPosition();
    void setModelRotation(glm::vec3 new_rot);
    glm::vec3 getModelRotation();
    void setModelScale(glm::vec3 new_scale);
    glm::vec3 getModelScale();

    void updateModel();
    void render(Camera *camera);
    bool init();
    void setColor(glm::vec3 color);
protected:
    void Common();
    void setDirtyMatrix();
private:
    // OpenGL context data
    Mesh *mesh;
    // Model data
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    bool dirty_matrix;
};

#endif // MODEL_H
