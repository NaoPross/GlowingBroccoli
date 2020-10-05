#include "snake.h"
#include <QDebug>

Snake::Snake() {
    tail = MIN_SIZE;

    for(int z=0;z<tail;z++) {

        y[z] = 0;
        x[z] = 0 - z*RECT_SIZE;

    }

    timerId = startTimer(DELAY);
}

QRectF Snake::boundingRect() const {
    return QRectF(0,0,800,800);
}

Snake::~Snake() {}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::red);
    for(int z=0;z<tail;z++) {
        painter->drawRect(x[z],y[z],RECT_SIZE,RECT_SIZE);
    }
}

void Snake::moveSnake() {
    for(int z=tail;z>0;z--) {
        x[z] = x[z-1];
        y[z] = y[z-1];
    }
        x[0] += RECT_SIZE;
}

void Snake::timerEvent(QTimerEvent *event) {

    Q_UNUSED(event);

    moveSnake();
    update();
}
