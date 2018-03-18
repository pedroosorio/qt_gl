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
    Model *model = new Model("");

    widget->setScene(scene);
    scene->attachModel(model);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), widget, SLOT(update()));
    timer->start(16);
}

void MainWindow::on_pushButton_clicked()
{
    scene->getModel(0)->setColor(glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                           static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                           static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
}
