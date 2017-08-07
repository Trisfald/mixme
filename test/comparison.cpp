#include <gtest/gtest.h>
#include <mixme/gift/comparison.hpp>
#include <mixme/detail/check/comparison.hpp>

using namespace mixme::gift;

namespace
{
    struct Got_less_than : comparison::all<Got_less_than>
	{
    	Got_less_than(int i) : i(i) {}
    	int i = 0;
	};
    bool operator<(const Got_less_than& lhs, const Got_less_than& rhs) { return lhs.i < rhs.i; }

    struct Got_greater_than : comparison::all<Got_greater_than>
	{
    	Got_greater_than(int i) : i(i) {}
    	int i = 0;
	};
    bool operator>(const Got_greater_than& lhs, const Got_greater_than& rhs) { return lhs.i > rhs.i; }

    struct Got_equal_to : comparison::all<Got_equal_to>
	{
    	Got_equal_to(int i) : i(i) {}
    	int i = 0;
	};
    bool operator==(const Got_equal_to& lhs, const Got_equal_to& rhs) { return lhs.i == rhs.i; }

    struct Got_not_equal_to : comparison::all<Got_not_equal_to>
	{
    	Got_not_equal_to(int i) : i(i) {}
    	int i = 0;
	};
    bool operator!=(const Got_not_equal_to& lhs, const Got_not_equal_to& rhs) { return lhs.i != rhs.i; }

    struct Got_less_than_equal_to : comparison::all<Got_less_than_equal_to>
	{
    	Got_less_than_equal_to(int i) : i(i) {}
    	int i = 0;
	};
    bool operator<=(const Got_less_than_equal_to& lhs, const Got_less_than_equal_to& rhs) { return lhs.i <= rhs.i; }

    struct Got_greater_than_equal_to : comparison::all<Got_greater_than_equal_to>
	{
    	Got_greater_than_equal_to(int i) : i(i) {}
    	int i = 0;
	};
    bool operator>=(const Got_greater_than_equal_to& lhs, const Got_greater_than_equal_to& rhs)
	{
    	return lhs.i >= rhs.i;
	}

    struct Overlap : comparison::eq<Overlap>, comparison::ne<Overlap>
	{
    	Overlap(int i) : i(i) {}
    	int i = 0;
	};
    bool operator<(const Overlap& lhs, const Overlap& rhs) { return lhs.i < rhs.i; }
    bool operator!=(const Overlap& lhs, const Overlap& rhs) { return lhs.i != rhs.i; }

    struct Composite : comparison::gt<Composite>
	{
    	Composite(int i) : i(i) {}
    	int i = 0;
	};
    bool operator<=(const Composite& lhs, const Composite& rhs) { return lhs.i <= rhs.i; }
}

TEST(COMPARISON, CHECKS_LAX)
{
	using namespace mixme::detail::check::comparison_lax;
	EXPECT_EQ(true, gt<Got_less_than>{}());
	EXPECT_EQ(true, lt<Got_greater_than>{}());
	EXPECT_EQ(true, ne<Got_equal_to>{}());
	EXPECT_EQ(true, eq<Got_not_equal_to>{}());
	EXPECT_EQ(true, ge<Got_less_than_equal_to>{}());
	EXPECT_EQ(true, le<Got_greater_than_equal_to>{}());
}

TEST(COMPARISON, CHECKS)
{
	using namespace mixme::detail::check::comparison;
	EXPECT_EQ(true, lt<Got_less_than>{}());
	EXPECT_EQ(true, gt<Got_greater_than>{}());
	EXPECT_EQ(true, eq<Got_equal_to>{}());
	EXPECT_EQ(true, ne<Got_not_equal_to>{}());
	EXPECT_EQ(true, le<Got_less_than_equal_to>{}());
	EXPECT_EQ(true, ge<Got_greater_than_equal_to>{}());
}

TEST(COMPARISON, GOT_LESS_THAN)
{
    Got_less_than one(1), two(2), three(3);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_LT(one, three);
    EXPECT_LE(one, two);
    EXPECT_LE(two, two);
    EXPECT_GT(two, one);
    EXPECT_GE(three, two);
    EXPECT_GE(three, three);
}

TEST(COMPARISON, GOT_GREATER_THAN)
{
	Got_less_than one(1), two(2), three(3);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_LT(one, three);
    EXPECT_LE(one, two);
    EXPECT_LE(two, two);
    EXPECT_GT(two, one);
    EXPECT_GE(three, two);
    EXPECT_GE(three, three);
}

TEST(COMPARISON, GOT_EQUAL_TO)
{
    Got_equal_to one(1), two(2);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_NE(two, one);
}

TEST(COMPARISON, GOT_NOT_EQUAL_TO)
{
    Got_not_equal_to one(1), two(2);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_NE(two, one);
}

TEST(COMPARISON, GOT_LESS_THAN_EQUAL_TO)
{
    Got_less_than_equal_to one(1), two(2), three(3);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_LT(one, three);
    EXPECT_LE(one, two);
    EXPECT_LE(two, two);
    EXPECT_GT(two, one);
    EXPECT_GE(three, two);
    EXPECT_GE(three, three);
}

TEST(COMPARISON, GOT_GREATER_THAN_EQUAL_TO)
{
    Got_greater_than_equal_to one(1), two(2), three(3);

    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_LT(one, three);
    EXPECT_LE(one, two);
    EXPECT_LE(two, two);
    EXPECT_GT(two, one);
    EXPECT_GE(three, two);
    EXPECT_GE(three, three);
}

TEST(COMPARISON, OVERLAP)
{
	Overlap one(1), two(2), three(3);
    EXPECT_EQ(one, one);
    EXPECT_NE(one, two);
    EXPECT_LT(one, three);
}

TEST(COMPARISON, COMPOSITION)
{
	Composite one(1), two(2);
	EXPECT_GT(two, one);
}
