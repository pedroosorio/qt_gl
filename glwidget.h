#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_0_Core>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "scene.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget() {}
    void setScene(Scene *s);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    Scene *scene;
};

#endif // GLWIDGET_H
