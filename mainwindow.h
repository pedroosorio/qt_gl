#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
private:
    Ui::MainWindow *ui;
    GLWidget *widget;
    RenderEngine *engine;

    //Temp
    QTimer *timer;
};

#endif // MAINWINDOW_H
