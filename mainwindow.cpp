#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLayout *central_layout = ui->centralWidget->layout();

    //Create QGLWidget
    widget = new GLWidget(parent);
    widget->setObjectName(QStringLiteral("glWidget"));
    central_layout->addWidget(widget);

    resize(640,480);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete widget;
    delete ui;
}

void MainWindow::init()
{
    scene = new Scene();
    Model *model = new Model("C:\\Users\\posorio\\Desktop\\teapot.obj");

    widget->setScene(scene);
    scene->attachModel(model);
}

void MainWindow::on_pushButton_clicked()
{
    scene->getModel(0)->setColor(glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                           static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                           static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    float camera_mov_const = 0.01f;
    //Basic camera controls
    switch(event->key()){

        case Qt::Key_W:{
            scene->translateCameraBy(glm::vec3(0.0f, -camera_mov_const, 0.0f));
            break;
        }
        case Qt::Key_A:{
            scene->translateCameraBy(glm::vec3(-camera_mov_const, 0.0f, 0.0f));
            break;
        }
        case Qt::Key_S:{
            scene->translateCameraBy(glm::vec3(0.0f, camera_mov_const, 0.0f));
            break;
        }
        case Qt::Key_D:{
            scene->translateCameraBy(glm::vec3(camera_mov_const, 0.0f, 0.0f));
            break;
        }
        case Qt::Key_Q:{
            scene->translateCameraBy(glm::vec3(0.0f, 0.0f, -camera_mov_const));
            break;
        }
        case Qt::Key_E:{
            scene->translateCameraBy(glm::vec3(0.0f, 0.0f, camera_mov_const));
            break;
        }
        case Qt::Key_Escape: {
            scene->getCamera()->reset();
            break;
        }
    }
}
