#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QPushButton>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* load custom font */
    int id = QFontDatabase::addApplicationFont(":/res/fonts/unscii-16.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont unscii_font(family);

    unscii_font.setBold(true);
    unscii_font.setPointSize(50);

    // set title font
    ui->titleLabel->setFont(unscii_font);

    /* game */

    // create a game
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    ui->graphicsView->setScene(scene);
    // ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    snake = new Snake;
    scene->addItem(snake);
    scene->installEventFilter(snake);

    ui->graphicsView->setSceneRect(snake->boundingRect());

    /* menu */

    // play button
    connect(ui->playBtn, &QPushButton::clicked, snake, [=]() {
        loadPage(MainWindow::Page::GAME);
        resizeGameToView();
        snake->startNewGame();
    });

    // exit button closes the window
    connect(ui->exitBtn, &QPushButton::clicked, snake, [=]() {
        close();
    });

    /* scoreboard */
    // TODO
}

MainWindow::~MainWindow()
{
    delete snake;
    delete scene;
    delete ui;
}

void MainWindow::loadPage(MainWindow::Page p) {
    ui->stackedWidget->setCurrentIndex(static_cast<int>(p));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    resizeGameToView();
}

void MainWindow::resizeGameToView() {
    QSize newSize = ui->graphicsView->size();
    float min = (newSize.width() > newSize.height()) ? newSize.height() : newSize.width();
    QRectF newRect(0, 0, min, min);

    snake->setGameState(Snake::GameState::INVALID);
    snake->updateBoundingRect(newRect);
    ui->graphicsView->setSceneRect(newRect);
    snake->setGameState(Snake::GameState::PLAY);
}
