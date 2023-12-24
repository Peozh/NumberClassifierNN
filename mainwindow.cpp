#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openGLWidget, &DrawPad28x28::inputUpdated, ui->widget, &PredictionBoard::updatePrediction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

