#include "camera.h"

QOpenGLFunctions_4_0_Core* GLctx;

Camera::Camera(): viewMatrix("view_matrix"), projMatrix("proj_matrix")
{
    fov = 45.0;
    aspectRatio = static_cast<float>(4.0/3.0);
    m_pos = glm::vec3(0.0,0.0,-1.7f);
    m_reset_pos = m_pos;
    m_orient = glm::toQuat(glm::orientate3(glm::vec3(0.0, 0.0, 0.0)));
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::Camera(glm::vec3 position, glm::quat orientation): viewMatrix("view_matrix"), projMatrix("proj_matrix")
{
    fov = 60.0;
    aspectRatio = static_cast<float>(4.0/3.0);
    m_pos = position;
    m_reset_pos = m_pos;
    m_orient = orientation;
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::~Camera()
{

}

// ---------------------------------------------------

void Camera::setPosition(glm::vec3 position)
{
    m_pos = position;
    updateViewMatrix();
}

void Camera::setOrientation(glm::quat orientation)
{
    m_orient = orientation;
    updateViewMatrix();
}

void Camera::setRotation(glm::vec3 rotation)
{
    m_orient = glm::toQuat(glm::orientate3(rotation));
    updateViewMatrix();
}

void Camera::setPose(glm::vec3 position, glm::quat orientation)
{
    m_pos = position;
    m_orient = orientation;
    updateViewMatrix();
}

void Camera::reset()
{
    m_pos = m_reset_pos;
    updateViewMatrix();
}

// ---------------------------------------------------

void Camera::lookAt(glm::vec3 look_at)
{
    viewMatrix.setProperty(glm::lookAt(m_pos, look_at, glm::vec3(0.0, 1.0, 0.0)));
}

void Camera::updateViewMatrix()
{
    glm::vec3 ypr = glm::eulerAngles(m_orient);
    viewMatrix.setProperty(glm::translate(glm::mat4(), m_pos)*glm::yawPitchRoll(ypr.x, ypr.y, ypr.z));
}

void Camera::updateProjectionMatrix()
{
    projMatrix.setProperty(glm::perspective(fov, aspectRatio, 0.01f, 100.0f));
}

void Camera::setAspectRatio(float as)
{
    aspectRatio = as;
    updateProjectionMatrix();
}

void Camera::setViewMatrixUniformLocation(GLuint location)
{
    viewMatrix.setUniformLocation(location);
}

void Camera::setProjMatrixUniformLocation(GLuint location)
{
    projMatrix.setUniformLocation(location);
}

void Camera::uploadMatricesToShader()
{
    viewMatrix.update();
    projMatrix.update();
}

glm::vec3 Camera::getPosition()
{
    return m_pos;
}

glm::quat Camera::getOrientation()
{
    return m_orient;
}

glm::vec3 Camera::getRotation()
{
    return glm::eulerAngles(m_orient);
}

// ---------------------------------------------------
