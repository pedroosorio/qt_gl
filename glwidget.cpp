#include "glwidget.h"
#include "glm/gtc/matrix_transform.hpp"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize( 4 );
    format.setSamples(24);
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(0);
    setFormat(format);
    engine = nullptr;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(engine) engine->init();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
}

void GLWidget::resizeGL(int width, int height)
{
    float retinaScale = devicePixelRatio();
    glViewport(GLint(0), GLint(0), GLsizei(width * retinaScale), GLsizei(height * retinaScale));
}

void GLWidget::render()
{
    if(!engine) { glClearColor(1.0, 1.0, 1.0, 1.0); return; }
    engine->renderMeshes();
}

// Camera functions
// ----------------------------------
void Camera::lookAt(pos look_at)
{
    viewMatrix = glm::lookAt(m_pos, look_at, glm::vec3(0.0, 1.0, 0.0));
}

void Camera::updateMatrix()
{
    glm::vec3 ypr = glm::eulerAngles(m_orient);
    viewMatrix = glm::translate(glm::mat4(), m_pos)*glm::yawPitchRoll(ypr.x, ypr.y, ypr.z);
}
// ----------------------------------
