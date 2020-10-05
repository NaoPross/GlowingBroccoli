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
    return QRectF(0,0,800,800); //may need to be changed to dynamic size!
}

Snake::~Snake() {}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::red); //set color of snake
    for(int z=0;z<tail;z++) {
        painter->drawRect(x[z],y[z],RECT_SIZE,RECT_SIZE); // draw snake
    }
}

void Snake::moveSnake() {
    for(int z=tail;z>0;z--) {
        x[z] = x[z-1];
        y[z] = y[z-1];
    }

    switch(direction) {
        case 1: //right
         x[0] += RECT_SIZE;
         break;
        case 2: //up
         y[0] -= RECT_SIZE;
         break;
        case 3: //left
         x[0] -= RECT_SIZE;
         break;
        case 4: //down
         y[0] += RECT_SIZE;
         break;
        }

    if(x[0]>WINDOW_SIZE) {
        x[0] = 0;
    }

}

void Snake::timerEvent(QTimerEvent *event) {

    Q_UNUSED(event);

    moveSnake();
    update();
}

bool Snake::eventFilter(QObject *obj, QEvent *event) //function for key press
 {
     if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         // qDebug("Key Press %d", keyEvent->key());

         switch(keyEvent->key()) {

            case 16777236: //arrow right
             direction = 1;
             break;
            case 16777235: //arrow up
             direction = 2;
             break;
            case 16777234: //arrow left
             direction = 3;
             break;
            case 16777237: //arrow down
             direction = 4;
             break;

         }

         return true;
     } else {
         return QObject::eventFilter(obj, event);
     }
 }
