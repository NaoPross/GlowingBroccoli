#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snake.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    Snake *snake = new Snake;
    scene->addItem(snake);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitGame() {

}



