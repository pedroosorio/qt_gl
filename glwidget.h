#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_0_Core>
#include "renderengine.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget() {}
    void attachRenderEngine(RenderEngine *re) { engine = re; re->setOpenGLContext(this);}
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void render();
private:
    RenderEngine *engine;
};

#endif // GLWIDGET_H
