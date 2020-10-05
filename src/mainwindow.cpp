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
    scene->installEventFilter(snake);

    //Add Grid
    QPixmap bg(20,20);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::white));
    p.drawRect(0, 0, 20, 20);
    scene->setBackgroundBrush(QBrush(bg));
}

MainWindow::~MainWindow()
{
    delete ui;
}
