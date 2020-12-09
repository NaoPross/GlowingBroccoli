#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>

/// \brief Graphic object that draws the game of Snake
///
/// The Snake class implements the *entire* game of snake. The rationale is to
/// keep the code simple.  Separation of graphical entities into separate object
/// types is premature optimization for a future extension that will not come.
/// Snake is a *very* simple game.
///
/// Usually QGraphicsObjects are not Q_OBJECTS by default for performance
/// reasons. In this case though performance is not a requirement, and being
/// able to use signals allows for a better integration with external widgets
/// (menu and scoreboard).
class Snake : public QGraphicsObject
{
    Q_OBJECT

public:
    /// \brief Minimal structure to hold informations about the score
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

    /// \brief Getter for the game state
    GameState gameState() const { return m_gameState; }

public slots:
    /// \brief Changes the internal state of the game (setter)
    /// \param s New state
    void setGameState(GameState s);

    /// \brief Updates the graphical bound in which the game is rendered
    /// \param rect New bounds
    void updateBoundingRect(const QRectF& rect);

    /// \brief Stops the game and resets the snake and the score
    /// \param playerName Name of the player that will be shown in the scoreboard
    void startNewGame(QString playerName);

    /// \brief Update the game
    void updateGame();

signals:
    /// \brief Emitted when a game finishes, i.e. the state changes to GameState::OVER
    /// \param Score The score reached by the player
    void gameOver(Score);
    /// \brief Emitted when the game state is changed to GameState::PAUSED
    void gamePaused();
    /// \brief Emitted when the game state is changed to GameState::PLAY
    void gameResumed();

protected:

    /// \sa QGraphicsObject::paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /// \sa QObject::eventFilter
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /// \brief Framerate of the game
    const unsigned m_fps = 12;
    /// \brief Font used by the painter
    QFont m_font;
    /// \brief Timer to advance the game
    QTimer m_timer;
    /// \brief Directions in which Snake can move
    enum class Direction : unsigned { UP, DOWN, LEFT, RIGHT };

    /// \brief Very simple internal structure to hold 2D coordinates
    struct Coordinate {
        int x, y;
        bool operator==(const Coordinate& other) const {
            return (x == other.x) && (y == other.y);
        }

        bool operator!=(const Coordinate& other) {
            return !(*this == other);
        }
    };

    /// \brief Internal game state
    GameState m_gameState = GameState::INVALID;
    /// \brief List of coordinates containing the snake body
    QVector<Coordinate> m_snake;
    /// \brief Direction in which the snake is moving
    Direction m_direction;
    /// \brief Coordinate of the next piece of food on the map
    Coordinate m_food;
    /// \brief Special food counter
    int m_sfood;
    /// \brief Current score of the player
    Score m_score;

    /// \brief Size of the grid for the game
    const int m_gridsize = 30;
    /// \brief (Graphical) Rectangle in which the game is drawn
    /// \sa Snake::updateBoundingRect
    QRectF m_gameRect = QRectF(0, 0, 200, 200);

    /// \brief Move the snake by 1 in a given direction
    /// \param d The Direction
    /// \sa Snake::Direction
    void moveSnake(Direction d);

    /// \brief Generate a new piece of food at random place on the map
    void generateFood();
};

#endif // SNAKE_H
