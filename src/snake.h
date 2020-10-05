#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>

/// \brief Helper class to manage a dynamically allocated grid
///
/// Grid is a non-copyable non-movable template class to
/// generate a dynamically allocated grid or matrix of type T
template<typename T>
class Grid
{
public:
    const unsigned width;
    const unsigned height;

    Grid() = delete;
    Grid(const Grid& other) = delete;
    Grid(Grid&& other) = delete;

    /// \brief Create a grid of width w and height h
    /// \param w the width / number of columns of the grid
    /// \param h the height / number of rows of the grid
    /// \param default_value the default value for the elements of the grid
    Grid(unsigned w, unsigned h, T default_value)
        : width(w), height(h)
    {
        grid = new T*[height];
        for (unsigned row = 0; row < height; row++) {
            grid[row] = new T[height];
            for (int col = 0; col < width; col++) {
                grid[row][col] = default_value;
            }
        }
    }

    ~Grid() {
        for (unsigned row = 0; row < height; row++) {
            delete[] grid[row];
        }

        delete[] grid;
    }

    /// \brief Get an element from the grid.
    /// \param row the row / y coordinate in the grid
    /// \param col the column / x coordinate in the grid
    /// \return Mutable reference to the cell
    T& get(unsigned row, unsigned col) {
        if (row < height && col < width) {
            return grid[row][col];
        }
        // TODO: throw an exception or change the rv to optional<T>
        return static_cast<T>(0);
    }

    /// \sa Grid<T>::get
    const T& get(unsigned row, unsigned col) const {
        return get(row, col);
    }

private:
    T **grid;
};

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

private:
    /// \brief Possible states of a cell.
    enum class CellT {
        EMPTY = 0, FOOD, SNAKE_HEAD, SNAKE_TAIL
    };

    /// \brief Direction in which snake is moving
    enum class Direction {
        SOUTH, EAST, NORTH, WEST
    };

    /// \brief Framerate of the game
    const unsigned fps = 60;
    /// \brief Update rate of the game
    const unsigned ups = 60;

    Grid<CellT> grid;
    Direction direction;

    /// \brief Update the game state
    void updateGame();
    /// \brief Draw the game state
    void drawGame();

    /// \brief Set the current direction in which Snake is moving
    /// \param d the Direction
    /// \sa Snake::Direction
    void moveSnake(Direction d);
};

#endif // SNAKE_H
