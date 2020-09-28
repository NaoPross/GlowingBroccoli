#include "snake.h"

Snake::Snake()
{

}

Snake::~Snake() {}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->drawRect(0,0,10,10);
}


void Snake::moveSnake() {
}

void Snake::timerEvent(QTimerEvent *event) {
    moveSnake();
    update();
}
