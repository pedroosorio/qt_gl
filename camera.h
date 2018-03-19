#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_0_Core>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "mesh.h"

class Camera {
public:
    Camera();
    Camera(glm::vec3 position, glm::quat orientation);
    ~Camera();

    // Camera setup functions
    void setPosition(glm::vec3 position);
    void setOrientation(glm::quat orientation);
    void setRotation(glm::vec3 rotation);
    void setPose(glm::vec3 position, glm::quat orientation);
    void reset();
    void lookAt(glm::vec3 look_at);
    void updateViewMatrix();
    void setViewMatrixUniformLocation(GLuint location);

    void updateProjectionMatrix();
    void setAspectRatio(float as);
    void setProjMatrixUniformLocation(GLuint location);
    void uploadMatricesToShader();

    // Camera information getters
    glm::vec3 getPosition();
    glm::quat getOrientation();
    glm::vec3 getRotation();
    glm::mat4 getCameraMatrix();
private:
    glm::vec3 m_pos, m_reset_pos;;
    glm::quat m_orient;
    Property<glm::mat4> viewMatrix;
    Property<glm::mat4> projMatrix;
    float fov;
    float aspectRatio;
    //Mesh *mesh;
};
#endif // CAMERA_H
