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
    widget->setScene(scene);

    Model *model = new Model("C:\\Users\\posorio\\Desktop\\teapot.obj");
    scene->attachModel(model);
}
