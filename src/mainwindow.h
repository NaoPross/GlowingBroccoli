#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "snake.h"

#include <QMainWindow>
#include <QGraphicsItem>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum class Page : int {
        MENU = 0, GAME = 1, SCOREBOARD = 2,
    };

public slots:
    void loadPage(Page p);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Snake *snake;

    void resizeGameToView();
};


#endif // MAINWINDOW_H
