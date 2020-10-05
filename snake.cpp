#include "snake.h"

Snake::Snake()
{

}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->drawRect(0,0,10,10);
}
