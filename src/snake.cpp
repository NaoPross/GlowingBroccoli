#include "snake.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QGraphicsScene>

Snake::Snake() {}

Snake::~Snake() {}

void Snake::setGameState(GameState state) {
    switch (state) {
    case GameState::PLAY:
        emit gameResumed();
        break;
    case GameState::PAUSED:
        emit gamePaused();
        break;
    case GameState::OVER:
        emit gameOver(m_score);
        break;
    case GameState::INVALID:
        // nothing special happens here
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

    /* start new timer */
    connect(&m_timer, &QTimer::timeout, this, &Snake::updateGame);
    m_timer.start(static_cast<int>(1000.0/m_fps));

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

    /* set up the rest */
    generateFood();

    Q_ASSERT(!m_snake.isEmpty());
    setGameState(GameState::PLAY);
};

// void Snake::gameOver(Snake::Score) {
//     disconnect(&m_timer, &QTimer::timeout, this, &Snake::updateGame);
//     //TODO: gameOver Behaviour
// }

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // TODO: cache this value
    const int cellWidth = static_cast<int>(m_gameRect.width() / static_cast<double>(m_gridsize));

    // draw game background
    painter->fillRect(m_gameRect, Qt::white);

    // draw snake
    painter->setBrush(Qt::red); // head color
    for (Coordinate coord : m_snake) {
        painter->drawRect(coord.x * cellWidth, coord.y * cellWidth, cellWidth, cellWidth);
        painter->setBrush(Qt::green); // body color
    }

    // draw food
    painter->setBrush(Qt::yellow); //food color
    painter->drawRect(m_food.x * cellWidth, m_food.y  * cellWidth, cellWidth, cellWidth);
}

void Snake::updateGame() {
    // render graphics
    update();

    // TODO: convert to switch?
    if (m_gameState == GameState::INVALID) {
        return;
    }

    if (m_gameState == GameState::PAUSED) {
        // wait
        return;
    }

    if (m_gameState == GameState::OVER) {
        // TODO: a special screen / something?
        setGameState(GameState::OVER);
        return;
    }

    Q_ASSERT(m_gameState == GameState::PLAY);

    // TODO: what does this if actually do?
    if(m_snake.count(m_snake.first())>1) {
        // FIXME
        // QTimer::singleShot(0, this, &Snake::gameOver);
        setGameState(GameState::OVER);
        return;
    }

    // TODO: collision checking

    // update snake
    moveSnake(m_direction);

    // condition to end the game
    if (m_snake.length() >= (m_gridsize * m_gridsize - 1)) {
        setGameState(GameState::OVER);
        return;
    }
}

void Snake::moveSnake(Direction d) {
    // TODO: check if the position of the snake is outside of the bounding region
    Coordinate head = m_snake.first();

    // TODO: move into updateGame()
    if((head.x > m_gridsize) || (head.x < 0) || (head.y > m_gridsize) || (head.y < 0)) {
        // FIXME
        // QTimer::singleShot(0, this, &Snake::gameOver);
        setGameState(GameState::OVER);
        return;
    }

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
            // TODO: make it a member or constant
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

         case Qt::Key_Escape:
             if (m_gameState == GameState::PAUSED)
                 setGameState(GameState::PLAY);
             else if (m_gameState == GameState::PLAY)
                 setGameState(GameState::PAUSED);
             break;
         }

         return true;
     } else {
         return QObject::eventFilter(obj, event);
     }
 }
