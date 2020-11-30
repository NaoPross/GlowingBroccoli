#include "snake.h"
#include <QDebug>
#include <QRandomGenerator>

Snake::Snake() {
    frameTimerId = startTimer(static_cast<int>(1000.0/fps));
    updateTimerId = startTimer(static_cast<int>(1000.0/ups));

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
}

Snake::~Snake() {}

QRectF Snake::boundingRect() const {
    // TODO: change to make dynamic
    return QRectF(0,0,800,800);
}

void Snake::timerEvent(QTimerEvent *event) {
    int eventTimerId = event->timerId();
    if (eventTimerId == frameTimerId) {
        // update graphics (calls paint)
        update();
    } else if (eventTimerId == updateTimerId) {
        updateGame();
    }
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: cache this value
    const int cellWidth = static_cast<int>(boundingRect().width() / static_cast<double>(gridsize));

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
    // TODO: collision checking

    // update snake
    moveSnake(direction);
}

void Snake::moveSnake(Direction d) {
    // TODO: check if the position of the snake is outside of the bounding region

    Coordinate head = snake.first();

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
    } while (snake.contains(food) == true);
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
