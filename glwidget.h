#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_4_0_Core>
#include "renderengine.h"

class Camera {
public:
    Camera(){
        m_pos = pos(0.0,0.0,0.0); m_orient = orient(1.0,0.0,0.0,0.0);
        updateMatrix();
    }
    Camera(pos position, orient orientation){
        m_pos = position; m_orient = orientation;
        updateMatrix();
    }
    ~Camera() {}

    // Camera setup functions
    void setCameraPosition(pos position) { m_pos = position; updateMatrix(); }
    void setCameraOrientation(orient orientation) { m_orient = orientation; updateMatrix(); }
    void setCameraRotation(rot rotation) { m_orient = glm::toQuat(glm::orientate3(rotation)); updateMatrix(); }
    void setCameraPose(pos position, orient orientation) { m_pos = position; m_orient = orientation; updateMatrix(); }
    void lookAt(pos look_at);
    void updateMatrix();

    // Camera information getters
    pos getPos();
    orient getOrientation();
    rot getRotation();
    glm::mat4 getCameraMatrix();
private:
    pos m_pos;
    orient m_orient;
    glm::mat4 viewMatrix;
};


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
    Camera main_cam;

};

#endif // GLWIDGET_H
