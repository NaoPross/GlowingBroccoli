#ifndef TST_GAMEPLAY_H
#define TST_GAMEPLAY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QObject>

#include "../src/snake.h"

using namespace testing;

TEST(GamePlay, State)
{
    Snake snake;
    ASSERT_THAT(snake.gameState(), Snake::GameState::INVALID);

    snake.startNewGame("Test Player");
    ASSERT_THAT(snake.gameState(), Snake::GameState::PLAY);
}

TEST(GamePlay, Score)
{
    Snake snake;
    Snake::Score testScore = {"Test Player", -1};

    QObject::connect(&snake, &Snake::gameOver, [&](Snake::Score s) {
        testScore.value = s.value;
    });

    snake.startNewGame("Test Player");
    snake.setGameState(Snake::GameState::OVER);

    ASSERT_THAT(testScore.value, 0);
}

#endif // TST_GAMEPLAY_H
