#include "snake.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QFont>
#include <QColor>

Snake::Snake() : m_font("monospace") {
    m_font.setStyleHint(QFont::Monospace);
    connect(&m_timer, &QTimer::timeout, this, &Snake::updateGame);
}

Snake::~Snake() {}

void Snake::setGameState(GameState state) {
    switch (state) {
    case GameState::PLAY:
        m_timer.start(static_cast<int>(1000.0/m_fps));
        qDebug("GameState::PLAY");
        emit gameResumed();
        break;
    case GameState::PAUSED:
        if (m_timer.isActive())
            m_timer.stop();
        update();
        qDebug("GameState::PLAY");
        emit gamePaused();
        break;
    case GameState::OVER:
        if (m_timer.isActive())
            m_timer.stop();
        qDebug("GameState::OVER");
        emit gameOver(m_score);
        break;
    case GameState::INVALID:
        qDebug("GameState::INVALID");
        if (m_timer.isActive())
            m_timer.stop();
        break;
    }
    m_gameState = state;
}

void Snake::updateBoundingRect(const QRectF& rect) {
    m_gameRect = rect;
}

void Snake::startNewGame(QString playerName) {
    setGameState(GameState::INVALID);

    /* reset score */
    m_score.player = playerName;
    m_score.value = 0;

    /* create a new snake */

    // delete any previous snake
    m_snake.clear();

    Coordinate head;
    // range to avoid spawning too close to the border
    const int range = 5;
    Q_ASSERT(range * 2 <= m_gridsize);
    do {
        head = {
            QRandomGenerator::global()->bounded(0, m_gridsize),
            QRandomGenerator::global()->bounded(0, m_gridsize)
        };
    } while (((head.x - range) > 0) && ((head.x + range) < m_gridsize)
          && ((head.y - range) > 0) && ((head.y + range) < m_gridsize));

    m_direction = static_cast<Direction>(QRandomGenerator::global()->bounded(0,4));

    m_snake.append(head);
    switch (m_direction) {
    case Direction::UP:
        m_snake.append({head.x, head.y +1});
        m_snake.append({head.x, head.y +2});
        break;
    case Direction::DOWN:
        m_snake.append({head.x, head.y -1});
        m_snake.append({head.x, head.y -2});
        break;
    case Direction::LEFT:
        m_snake.append({head.x +1, head.y});
        m_snake.append({head.x +2, head.y});
        break;
    case Direction::RIGHT:
        m_snake.append({head.x -1, head.y});
        m_snake.append({head.x -2, head.y});
        break;
    }

    /* create food */
    generateFood();

    /* start game */
    Q_ASSERT(!m_snake.isEmpty());
    setGameState(GameState::PLAY);
};

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: cache this value
    const int cellWidth = static_cast<int>(m_gameRect.width() / static_cast<double>(m_gridsize));

    // draw game background
    painter->fillRect(m_gameRect, QColor(0xF6F1D1));

    // draw snake
    painter->setBrush(QColor(0xFF784F)); // head color
    for (Coordinate coord : m_snake) {
        painter->drawRect(coord.x * cellWidth, coord.y * cellWidth, cellWidth, cellWidth);
        painter->setBrush(QColor(0x5B7553)); // body color
    }

    // draw food
    painter->setBrush(QColor(0x006989)); //food color
    painter->drawRect(m_food.x * cellWidth, m_food.y  * cellWidth, cellWidth, cellWidth);

    // draw score
    const int textHeight = cellWidth * 2;
    m_font.setPixelSize(textHeight); // same as below
    painter->setFont(m_font);

    // TODO(improvement): move the score when the snake and/or food is under it
    Coordinate scorePos = {1, 1};
    const QRect scoreRect = QRect(
        scorePos.x * cellWidth, scorePos.y * cellWidth,
        cellWidth * 6, textHeight);
    painter->setPen(QColor(0x00000F));

    painter->drawText(
        scoreRect,
        Qt::AlignCenter,
        QString("%1").arg(m_score.value, 6, 10, QLatin1Char('0'))
    );

    if (gameState() == GameState::PAUSED) {
        painter->setPen(QColor(0x00000F));
        const int pauseTextHeight = cellWidth * 5;
        m_font.setPixelSize(pauseTextHeight);
        painter->setFont(m_font);
        const QRect pauseRect = QRect(
            0, (m_gameRect.height() - pauseTextHeight) /2,
            m_gameRect.width(), pauseTextHeight
        );
        painter->drawText(
            pauseRect,
            Qt::AlignCenter,
            "PAUSED"
        );

    }

}

void Snake::updateGame() {
    // render graphics
    update();
    Q_ASSERT(m_gameState == GameState::PLAY);

    // when the snake eats itself
    if(m_snake.count(m_snake.first()) > 1) {
        setGameState(GameState::OVER);
        return;
    }

    // when a border is hit
    Coordinate head = m_snake.first();
    if((head.x > m_gridsize) || (head.x < 0) || (head.y > m_gridsize) || (head.y < 0)) {
        setGameState(GameState::OVER);
        return;
    }

    // when the snake is long enough to fill the entire map
    if (m_snake.length() >= (m_gridsize * m_gridsize - 1)) {
        setGameState(GameState::OVER);
        return;
    }

    // update snake
    moveSnake(m_direction);
}

void Snake::moveSnake(Direction d) {
    Coordinate head = m_snake.first();

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

    if (head != m_snake.at(1)) {
        m_snake.prepend(head);
        if (head == m_food) {
            m_score.value += 100;
            generateFood();
        } else {
            m_snake.removeLast();
        }
    }
}

void Snake::generateFood() {
    do {
        m_food = {
            QRandomGenerator::global()->bounded(0, m_gridsize),
            QRandomGenerator::global()->bounded(0, m_gridsize)
        };
    } while (m_snake.contains(m_food));
}

bool Snake::eventFilter(QObject *obj, QEvent *event) {
     if (event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

         if (keyEvent->key() == Qt::Key_Escape) {
             if (m_gameState == GameState::PAUSED)
                 setGameState(GameState::PLAY);
             else if (m_gameState == GameState::PLAY)
                 setGameState(GameState::PAUSED);

             return true;
         }

         if (m_gameState != GameState::PLAY) {
             return true;
         }

         switch(keyEvent->key()) {
         case Qt::Key_Right:
         case Qt::Key_D:
             if (m_direction != Direction::LEFT)
                 m_direction = Direction::RIGHT;
             break;

         case Qt::Key_Up:
         case Qt::Key_W:
             if (m_direction != Direction::DOWN)
                 m_direction =  Direction::UP;
             break;

         case Qt::Key_Left:
         case Qt::Key_A:
             if (m_direction != Direction::RIGHT)
                 m_direction = Direction::LEFT;
             break;

         case Qt::Key_Down:
         case Qt::Key_S:
             if (m_direction != Direction::UP)
                 m_direction = Direction::DOWN;
             break;
         }

         return true;
     } else {
         return QObject::eventFilter(obj, event);
     }
 }
