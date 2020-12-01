#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>

/// \brief Graphic object that draws the game of Snake
class Snake : public QGraphicsObject
{
public:
    /// \brief Possible states of the game
    enum class GameState : unsigned { PLAY, PAUSED, OVER, INVALID };

    Snake();
    virtual ~Snake();

    /// \sa QGrpahicsObject::boundingRect
    QRectF boundingRect() const override;

public slots:
    void setGameState(GameState s);
    void updateBoundingRect(const QRectF& rect);
    void startNewGame();
    void gameOver();

protected:

    /// \sa QGraphicsObject::paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /// \sa QObject::eventFilter
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /// \brief Framerate of the game
    const unsigned fps = 60;
    /// \brief Timer to advance the game
    QTimer timer;
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

    GameState gameState = GameState::PAUSED;
    QVector<Coordinate> snake;
    Direction direction;
    Coordinate food;

    const int gridsize = 30;
    QRectF gameRect = QRectF(0, 0, 200, 200);

    /// \brief Update the game state
    void updateGame();

    /// \brief Move the snake by 1 in a given direction
    /// \param d the Direction
    /// \sa Snake::Direction
    void moveSnake(Direction d);

    /// \brief Generate a new piece of food at random place on the map
    void generateFood();
};

#endif // SNAKE_H
