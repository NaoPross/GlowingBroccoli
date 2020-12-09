#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QPushButton>
#include <QLineEdit>
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

    ui->playBtn->setEnabled(false);
    connect(ui->playerNameEdit, &QLineEdit::textChanged, this, [=]() {
        ui->playBtn->setEnabled(!ui->playerNameEdit->text().isEmpty());
    });

    // play button
    connect(ui->playBtn, &QPushButton::clicked, [=]() {
        loadPage(MainWindow::Page::GAME);
        resizeGameToView();
        snake->startNewGame(ui->playerNameEdit->text());
    });

    // scoreboard button
    connect(ui->scoreboardBtn, &QPushButton::clicked, [=]() {
        loadPage(MainWindow::Page::SCOREBOARD);
    });

    // exit button closes the window
    connect(ui->exitBtn, &QPushButton::clicked, this, &QMainWindow::close);

    /* scoreboard */
    connect(snake, &Snake::gameOver, this, &MainWindow::addScore);
    connect(ui->backToMenuBtn, &QPushButton::clicked, [=]() {
        loadPage(MainWindow::Page::MENU);
    });
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

void MainWindow::addScore(Snake::Score s) {
    scoreboard.append(s);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    resizeGameToView();
}

void MainWindow::resizeGameToView() {
    QSize newSize = ui->graphicsView->size();
    float min = (newSize.width() > newSize.height()) ? newSize.height() : newSize.width();
    QRectF newRect(0, 0, min, min);

    Snake::GameState state = snake->gameState();

    snake->setGameState(Snake::GameState::INVALID);
    snake->updateBoundingRect(newRect);
    ui->graphicsView->setSceneRect(newRect);
    snake->setGameState(state);
}
