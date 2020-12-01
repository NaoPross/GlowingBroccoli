#include "snake.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QGraphicsScene>

Snake::Snake() {}

Snake::~Snake() {}

void Snake::setGameState(GameState state) {
    gameState = state;
}

void Snake::updateBoundingRect(const QRectF& rect) {
    gameRect = rect;
}

QRectF Snake::boundingRect() const {
    return gameRect;
}

void Snake::startNewGame() {

    connect(&timer, &QTimer::timeout, this, &Snake::updateGame);
    timer.start(static_cast<int>(fps));

    Coordinate head = {
        QRandomGenerator::global()->bounded(0, gridsize),
        QRandomGenerator::global()->bounded(0, gridsize)
    };

    direction = static_cast<Direction>(QRandomGenerator::global()->bounded(0,4));

    snake.append(head);
    switch (direction) {
    case Direction::UP:
        snake.append({head.x, head.y +1});
        snake.append({head.x, head.y +2});
        break;
    case Direction::DOWN:
        snake.append({head.x, head.y -1});
        snake.append({head.x, head.y -2});
        break;
    case Direction::LEFT:
        snake.append({head.x +1, head.y});
        snake.append({head.x +2, head.y});
        break;
    case Direction::RIGHT:
        snake.append({head.x -1, head.y});
        snake.append({head.x -2, head.y});
        break;
    }

    generateFood();
    gameState = GameState::PLAY;
};

void Snake::gameOver() {
    disconnect(&timer, &QTimer::timeout, this, &Snake::updateGame);
    //TODO: gameOver Behaviour
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: cache this value
    const int cellWidth = static_cast<int>(gameRect.width() / static_cast<double>(gridsize));

    // draw game background
    painter->fillRect(gameRect, Qt::white);

    // draw snake
    painter->setBrush(Qt::red); // head color
    for (Coordinate coord : snake) {
        painter->drawRect(coord.x * cellWidth, coord.y * cellWidth, cellWidth, cellWidth);
        painter->setBrush(Qt::green); // body color
    }

    // draw food
    painter->setBrush(Qt::yellow); //food color
    painter->drawRect(food.x * cellWidth, food.y  * cellWidth, cellWidth, cellWidth);
}

void Snake::updateGame() {
    //redraw graphics
    update();

    if (gameState != GameState::PLAY) {
        qDebug("not updating game!");
        return;
    }

    if(snake.count(snake.first())>1) {
        QTimer::singleShot(0, this, &Snake::gameOver);
    }

    // update snake
    moveSnake(direction);
}

void Snake::moveSnake(Direction d) {

    Coordinate head = snake.first();

    if((head.x > gridsize) || (head.x < 0) || (head.y > gridsize) || (head.y < 0)) {
        QTimer::singleShot(0, this, &Snake::gameOver);
    }

    switch (d) {
        case Direction::UP:  head.y -= 1;
            break;
        case Direction::DOWN: head.y += 1;
            break;
        case Direction::LEFT: head.x -= 1;
            break;
        case Direction::RIGHT: head.x += 1;
            break;
    }

    if (head != snake.at(1)) {
        snake.prepend(head);
        if (head == food) {
            generateFood();
        } else {
            snake.removeLast();
        }
    }
}

void Snake::generateFood() {
    do {
        food = {
            QRandomGenerator::global()->bounded(0, gridsize),
            QRandomGenerator::global()->bounded(0, gridsize)
        };
    } while (snake.contains(food));
}

bool Snake::eventFilter(QObject *obj, QEvent *event) {
     if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

         switch(keyEvent->key()) {
         case Qt::Key_Right:
         case Qt::Key_D:

             if (direction != Direction::LEFT) {
                 direction = Direction::RIGHT;
             }

             break;

         case Qt::Key_Up:
         case Qt::Key_W:

             if (direction != Direction::DOWN) {
                 direction =  Direction::UP;
             }
             break;

         case Qt::Key_Left:
         case Qt::Key_A:

             if (direction != Direction::RIGHT) {
                 direction = Direction::LEFT;
             }
             break;

         case Qt::Key_Down:
         case Qt::Key_S:

             if (direction != Direction::UP) {
                 direction = Direction::DOWN;
             }
             break;
         }

         return true;
     } else {
         return QObject::eventFilter(obj, event);
     }
 }
