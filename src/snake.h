#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>

class Snake : public QGraphicsObject
{
public:
    Snake();
    virtual ~Snake();

    QRectF boundingRect () const override;

protected:
    void timerEvent(QTimerEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    static const int RECT_SIZE = 20;
    static const int MAX_SIZE = 100;
    static const int MIN_SIZE = 3;
    static const int DELAY = 120;

    int x[MAX_SIZE];
    int y[MAX_SIZE];
    int direction;
    int tail;
    int timerId;

    void moveSnake();
};

#endif // SNAKE_H
