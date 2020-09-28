#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>

class Snake : public QGraphicsItem
{
public:
    Snake();
    virtual ~Snake();

    QRectF boundingRect () const {
        return QRectF(-300,-300,600,600);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void timerEvent(QTimerEvent *event);

private:
    static const int MAX_SIZE = 100;

    int x[MAX_SIZE];
    int y[MAX_SIZE];

    void moveSnake();
};

#endif // SNAKE_H
