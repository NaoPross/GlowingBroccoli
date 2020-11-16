#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>
#include <QVector>

/// \brief Graphic object that draws the game of Snake
class Snake : public QGraphicsObject
{
public:
    Snake();
    virtual ~Snake();

    /// \sa QGrpahicsObject::boundingRect
    QRectF boundingRect() const override;

protected:
    /// \sa QObject::timerEvent
    void timerEvent(QTimerEvent *event) override;

    /// \sa QGraphicsObject::paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /// \sa QObject::eventFilter
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /// \brief Framerate of the game
    const unsigned fps = 60;
    /// \brief Update rate of the game
    const unsigned ups = 15;

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


    QVector<Coordinate> snake;
    Direction direction;
    Coordinate food;
    const int gridsize = 60;

    /// \brief Update the game state
    void updateGame();

    /// Timer id for frame update
    int frameTimerId = -1;
    /// Timer id for game update
    int updateTimerId = -1;

    /// \brief Move the snake by 1 in a given direction
    /// \param d the Direction
    /// \sa Snake::Direction
    void moveSnake(Direction d);

    /// \brief Move the snake in a given direction
    /// \param d the direction
    /// \param howmany how many steps
    /// \sa Snake::Direction
    void moveSnake(Direction d, unsigned howmany);
};

#endif // SNAKE_H
