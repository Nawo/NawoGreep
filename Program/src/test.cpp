#include <algorithm>
#include <cmath>
#include <vector>

#include "Grep.hpp"
#include "gtest/gtest.h"

TEST(countPatternInLineTest, ShouldCountPattern) {
    Grep cut;
    ASSERT_EQ(cut.countPatternInLine("i like Julia very hihi!", "hihi"), 1);
    ASSERT_EQ(cut.countPatternInLine("is this good option is?", "is"), 2);
    ASSERT_EQ(cut.countPatternInLine("Dupa is dupa, is, dupa is", "is"), 3);
    ASSERT_EQ(cut.countPatternInLine("dupadupa haha", "dupa"), 0);
    ASSERT_EQ(cut.countPatternInLine("haha", "haha"), 1);
    ASSERT_EQ(cut.countPatternInLine("hahahaha", "haha"), 0);
}