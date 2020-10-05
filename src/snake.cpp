#include "snake.h"
#include <QDebug>

Snake::Snake() : direction(Direction::RIGHT) {
    frameTimerId = startTimer(static_cast<int>(1000.0/fps));
    updateTimerId = startTimer(static_cast<int>(1000.0/ups));

    // TODO: generate randomly the first coordinate
    // TODO: remove test code
    snake.append({10, 2});
    snake.append({11, 2});
}

Snake::~Snake() {}

QRectF Snake::boundingRect() const {
    return QRectF(0,0,800,800);
}

void Snake::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
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

    // TODO: cache this value, make 100 a parameter
    const int cellWidth = static_cast<int>(boundingRect().width() / 100.);

    // draw snake
    painter->setBrush(Qt::red); // head color
    for (Coordinate coord : snake) {
        painter->drawRect(coord.x * cellWidth, coord.y * cellWidth, cellWidth, cellWidth);
        painter->setBrush(Qt::green); // body color
    }
}

void Snake::updateGame() {
    // TODO: collision checking

    // update snake
    moveSnake(direction, 1);
}

void Snake::moveSnake(Direction d) {
    switch (d) {
        case Direction::UP:
            snake.front().y -= 1;
            break;

        case Direction::DOWN:
            snake.front().y += 1;
            break;

        case Direction::LEFT:
            snake.front().x -= 1;
            break;

        case Direction::RIGHT:
            snake.front().x += 1;
            break;
    }

    // TODO: move the rest of the body to follow the head
}

void Snake::moveSnake(Direction d, unsigned howmany) {
    while (howmany--) {
        moveSnake(d);
    }
}
