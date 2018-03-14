#include "glwidget.h"

GLuint vbo;

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
