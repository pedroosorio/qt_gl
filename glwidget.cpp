#include "glwidget.h"

#define REQUIRED_RENDER_PERIOD 16
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

int32_t GLWidget::computeFPS()
{
    int32_t elapsedms = timer.elapsed();
    timer.start();
    static int counter = 0;
    static float fpsCounter = 0.0;
    if(elapsedms > 0 && elapsedms < 100){
        fpsCounter+=elapsedms;
        counter++;
        if(counter > 20) {
            avgFPS = 1000.0/(fpsCounter/float(counter));
            counter=0;
            fpsCounter=0.0;
            qDebug() << avgFPS;
        }
    } else return 0;
    return elapsedms;
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
    connect(&renderTimer, SIGNAL(timeout()), this, SLOT(update()));
    renderTimer.start(REQUIRED_RENDER_PERIOD);
}

void GLWidget::paintGL()
{
    renderTimer.stop();
    int32_t elapsed = computeFPS();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!scene) { glClearColor(1.0, 1.0, 1.0, 1.0); return; }
    scene->renderScene();
    renderTimer.start(REQUIRED_RENDER_PERIOD);
}

void GLWidget::resizeGL(int width, int height)
{
    float retinaScale = devicePixelRatio();
    glViewport(GLint(0), GLint(0), GLsizei(width * retinaScale), GLsizei(height * retinaScale));
    scene->getCamera()->setAspectRatio(float(width)/float(height));
}

// ---------------------------------------------------
