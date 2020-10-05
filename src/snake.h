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
    void timerEvent(QTimerEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    static const int RECT_SIZE = 20; //size of one snake part
    static const int MAX_SIZE = 100; //maximum snake length
    static const int MIN_SIZE = 3; //initial length of snake
    static const int DELAY = 120; // speed of timer
    static const int WINDOW_SIZE = 800; //window size set in designer

    int x[MAX_SIZE];
    int y[MAX_SIZE];
    int direction;
    int tail;
    int timerId;

    void moveSnake();
};

#endif // SNAKE_H
