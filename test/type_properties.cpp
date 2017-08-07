#include <gtest/gtest.h>
#include <mixme/gift/type_properties.hpp>

using namespace mixme::gift;

namespace
{
	struct Copy : copy_only {};
	struct Move : move_only {};
	struct No : no_copy_or_move {};
}

TEST(TYPE_PROPERTIES, NO_COPY_OR_MOVE)
{
    EXPECT_FALSE(std::is_copy_assignable<No>::value);
    EXPECT_FALSE(std::is_copy_constructible<No>::value);
    EXPECT_FALSE(std::is_move_assignable<No>::value);
    EXPECT_FALSE(std::is_move_constructible<No>::value);
}

TEST(TYPE_PROPERTIES, COPY_ONLY)
{
    EXPECT_TRUE(std::is_copy_assignable<Copy>::value);
    EXPECT_TRUE(std::is_copy_constructible<Copy>::value);
    EXPECT_TRUE(std::is_move_assignable<Copy>::value); // can still bind to a temporary
    EXPECT_TRUE(std::is_move_constructible<Copy>::value); // can still bind to a temporary
}

TEST(TYPE_PROPERTIES, MOVE_ONLY)
{
    EXPECT_TRUE(!std::is_copy_assignable<Move>::value);
    EXPECT_TRUE(!std::is_copy_constructible<Move>::value);
    EXPECT_TRUE(std::is_move_assignable<Move>::value);
    EXPECT_TRUE(std::is_move_constructible<Move>::value);
}
