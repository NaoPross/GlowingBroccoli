#ifndef TST_GAMESTATE_H
#define TST_GAMESTATE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(GamePlay, GameState)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_GAMESTATE_H
