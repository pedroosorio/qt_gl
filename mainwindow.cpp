#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    delete engine;
    delete widget;
    delete ui;
}

void MainWindow::init()
{
    engine = new RenderEngine();
    Model *model = new Model("");

    widget->attachRenderEngine(engine);
    engine->attachModel(model);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), widget, SLOT(update()));
    timer->start(16);
}
