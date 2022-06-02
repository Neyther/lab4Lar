#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonNext, &QPushButton::clicked, ui->openGLWidget, &glScene::next);
    connect(ui->pushButtonPrev, &QPushButton::clicked, ui->openGLWidget, &glScene::prev);

}

MainWindow::~MainWindow()
{
    delete ui;
}
