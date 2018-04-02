#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_0_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "scene.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget() {}
    void setScene(Scene *s);
protected:
    int32_t computeFPS();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
public slots:
    // Input handler events
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    Scene *scene;
    QTimer renderTimer;
    QElapsedTimer timer;
    float avgFPS;
    // Freetype variables
    FT_Library library;
    FT_Face face;
};

#endif // GLWIDGET_H
