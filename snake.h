#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>

class Snake
{
public:
    Snake();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // SNAKE_H
