#include "snake.h"
#include <QDebug>

Snake::Snake() : direction(Direction::RIGHT) {
    frameTimerId = startTimer(static_cast<int>(1000.0/fps));
    updateTimerId = startTimer(static_cast<int>(1000.0/ups));

    // TODO: generate randomly the first coordinate
    // TODO: remove test code
    snake.append({12, 2});
    snake.append({11, 2});
    snake.append({10, 2});
    snake.append({9, 2});
    snake.append({8, 2});
    snake.append({7, 2});
    snake.append({6, 2});
    snake.append({5, 2});
    snake.append({4, 2});

    food = {50,50};
}

Snake::~Snake() {}

QRectF Snake::boundingRect() const {
    return QRectF(0,0,800,800); //may need to be changed to dynamic size!
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

    // TODO: cache this value, make 100 a parameter
    const int cellWidth = static_cast<int>(boundingRect().width() / 60.);

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

    //...
    if (head != snake.at(1)) {
        snake.prepend(head);


        if (head == food) {

            do {
                food = {rand()%10, rand()%10};
            } while (snake.contains(food) == true);

        } else {
            snake.removeLast();
        }

    }

}

void Snake::moveSnake(Direction d, unsigned howmany) {
    while (howmany--) {
        moveSnake(d);
    }
}

bool Snake::eventFilter(QObject *obj, QEvent *event) //function for key press
 {
     if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

         switch(keyEvent->key()) {
             case Qt::Key_Right:
             case Qt::Key_D:
                 direction = Direction::RIGHT;
                 break;

             case Qt::Key_Up:
             case Qt::Key_W:
                 direction =  Direction::UP;
                 break;

             case Qt::Key_Left:
             case Qt::Key_A:
                 direction = Direction::LEFT;
                 break;

             case Qt::Key_Down:
             case Qt::Key_S:
                 direction = Direction::DOWN;
                 break;
         }

         return true;
     } else {
         return QObject::eventFilter(obj, event);
     }
 }
