#include "snake.h"
#include <QDebug>

Snake::Snake() : grid(80, 80, Snake::CellT::EMPTY) {
    frameTimerId = startTimer(static_cast<int>(1000.0/fps));
    updateTimerId = startTimer(static_cast<int>(1000.0/ups));

    grid.get(10, 0) = CellT::SNAKE_HEAD;
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

    // TODO: cache
    int rect_width = boundingRect().width() / grid.width;
    int rect_height = boundingRect().height() / grid.height;

    for (unsigned row = 0; row < grid.rows; row++) {
        for (unsigned col = 0; col < grid.cols; col++) {
            switch (grid.get(row, col)) {
            case CellT::EMPTY:
                painter->setPen(Qt::white);
                painter->setBrush(Qt::white);
                break;

            case CellT::FOOD:
                painter->setBrush(Qt::red);
                break;

            case CellT::SNAKE_HEAD:
                painter->setBrush(Qt::blue);
                break;

            case CellT::SNAKE_TAIL:
                painter->setBrush(Qt::green);
                break;
            }

            painter->drawRect(col * rect_width, row * rect_height, rect_width, rect_height);
        }
    }
}

void Snake::updateGame() {
    bool hasFood = false;
    // FIXME: this does not work for more advanced changes
    bool skipNextCell = false;

    for (unsigned row = 0; row < grid.rows; row++) {
        for (unsigned col = 0; col < grid.cols; col++) {
            if (skipNextCell) {
                skipNextCell = false;
                continue;
            }

            switch (grid.get(row, col)) {
            case CellT::EMPTY:
                // do nothing
                break;

            case CellT::FOOD:
                hasFood = true;
                break;

            case CellT::SNAKE_HEAD:
                grid.get(row, col) = CellT::EMPTY;
                grid.get(row, col +1) = CellT::SNAKE_HEAD;
                skipNextCell = true;
                break;

            case CellT::SNAKE_TAIL:
                break;
            }
        }
    }

    // TODO
    if (!hasFood) {
    }
}
