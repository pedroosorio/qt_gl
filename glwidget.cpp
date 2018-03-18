#include "glwidget.h"

extern QOpenGLFunctions_4_0_Core *GLctx;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize( 4 );
    format.setSamples(24);
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(0);
    setFormat(format);
    GLctx = this;
    scene = nullptr;
}

void GLWidget::setScene(Scene *s)
{
    scene = s;
}

// ---------------------------------------------------

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(scene) scene->init();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!scene) { glClearColor(1.0, 1.0, 1.0, 1.0); return; }
    scene->renderScene();
}

void GLWidget::resizeGL(int width, int height)
{
    float retinaScale = devicePixelRatio();
    glViewport(GLint(0), GLint(0), GLsizei(width * retinaScale), GLsizei(height * retinaScale));
}

// ---------------------------------------------------
