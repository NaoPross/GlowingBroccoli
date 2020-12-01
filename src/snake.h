#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>
#include <QVector>

/// \brief Graphic object that draws the game of Snake
class Snake : public QGraphicsObject
{
    Q_OBJECT

public:
    struct Score {
        QString player;
        int value;
    };

    /// \brief Possible states of the game
    enum class GameState : unsigned { PLAY, PAUSED, OVER, INVALID };

    Snake();
    virtual ~Snake();

    /// \sa QGrpahicsObject::boundingRect
    QRectF boundingRect() const override { return m_gameRect; }
    GameState gameState() const { return m_gameState; }

public slots:
    void setGameState(GameState s);
    void updateBoundingRect(const QRectF& rect);
    void startNewGame(QString playerName);

signals:
    void gameOver(Score);
    void gamePaused();
    void gameResumed();

protected:
    /// \sa QObject::timerEvent
    void timerEvent(QTimerEvent *event) override;

    /// \sa QGraphicsObject::paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /// \sa QObject::eventFilter
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /// \brief Framerate of the game
    const unsigned m_fps = 60;
    /// \brief Update rate of the game
    const unsigned m_ups = 15;

    /// \brief Directions in which Snake can move
    enum class Direction : unsigned { UP, DOWN, LEFT, RIGHT };

    /// \brief Very simple structure to hold 2D coordinates
    struct Coordinate {
        int x, y;
        bool operator==(const Coordinate& other) const {
            return (x == other.x) && (y == other.y);
        }

        bool operator!=(const Coordinate& other) {
            return !(*this == other);
        }
    };

    GameState m_gameState = GameState::INVALID;
    QVector<Coordinate> m_snake;
    Direction m_direction;
    Coordinate m_food;
    Score m_score;

    const int m_gridsize = 30;
    QRectF m_gameRect = QRectF(0, 0, 200, 200);

    /// \brief Update the game state
    void updateGame();

    /// Timer id for frame update
    int m_frameTimerId = -1;
    /// Timer id for game update
    int m_updateTimerId = -1;

    /// \brief Move the snake by 1 in a given direction
    /// \param d the Direction
    /// \sa Snake::Direction
    void moveSnake(Direction d);

    /// \brief Generate a new piece of food at random place on the map
    void generateFood();
};

#endif // SNAKE_H
