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
    unscii_font = QFont(family);
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
    snake->setFont(unscii_font);

    scene->addItem(snake);
    scene->installEventFilter(snake);

    ui->graphicsView->setSceneRect(snake->boundingRect());

    /* menu */

    // game can be player only when a name is set
    ui->playBtn->setEnabled(false);
    connect(ui->playerNameEdit, &QLineEdit::textChanged, this, [=]() {
        ui->playBtn->setEnabled(!ui->playerNameEdit->text().isEmpty());
    });

    // play button
    connect(ui->playBtn, &QPushButton::clicked, [=]() {
        // FIXME: the page is loaded twice because resizeGameToView uses
        // Snake::setGameState() which emits signals that in some edge cases
        // cause the page to be changed. This is workaround to temporarely fix
        // the problem.
        loadPage(MainWindow::Page::GAME);
        resizeGameToView();

        loadPage(MainWindow::Page::GAME);
        snake->startNewGame(ui->playerNameEdit->text());
    });

    // scoreboard button
    connect(ui->scoreboardBtn, &QPushButton::clicked, [=]() {
        loadPage(MainWindow::Page::SCOREBOARD);
    });

    // exit button closes the window
    connect(ui->exitBtn, &QPushButton::clicked, this, &QMainWindow::close);

    /* scoreboard */
    // TODO: improve sorting: should be with the score (this is probably lexicographical)
    ui->scoreboardListWidget->setSortingEnabled(true);

    // gameOver signal from the game shows the scoreboard
    connect(snake, &Snake::gameOver, [=](Snake::Score s) {
        snake->setGameState(Snake::GameState::INVALID);
        addScore(s);
        loadPage(MainWindow::Page::SCOREBOARD);
    });

    // return to menu from scoreboard
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
    Q_ASSERT(ui->stackedWidget->widget(static_cast<int>(MainWindow::Page::MENU)) == ui->menuPage);
    Q_ASSERT(ui->stackedWidget->widget(static_cast<int>(MainWindow::Page::GAME)) == ui->gamePage);
    Q_ASSERT(ui->stackedWidget->widget(static_cast<int>(MainWindow::Page::SCOREBOARD)) == ui->scoreboardPage);

    ui->stackedWidget->setCurrentIndex(static_cast<int>(p));
}

void MainWindow::addScore(Snake::Score s) {
    QString score = QString("%1 - ").arg(s.value, 6, 10, QLatin1Char('0'));
    QListWidgetItem *item = new QListWidgetItem(score + s.player, ui->scoreboardListWidget);
    unscii_font.setPointSize(15);
    item->setFont(unscii_font);
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
