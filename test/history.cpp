#include <gtest/gtest.h>
#include <mixme/wrap/history.hpp>

using namespace mixme::wrap;

namespace
{
    struct Simple_type
    {
        Simple_type() = default;
        Simple_type(int i, float f) : i(i), f(f) {}
        int i = 0;
        float f = 0.0f;
    };

    bool operator==(const Simple_type& lhs, const Simple_type& rhs)
    {
        return lhs.i == rhs.i && lhs.f == rhs.f;
    }

    bool operator!=(const Simple_type& lhs, const Simple_type& rhs)
    {
        return lhs.i != rhs.i || lhs.f != rhs.f;
    }

    struct Move_only_type
    {
        Move_only_type() = default;
        Move_only_type(int i, float f) : i(i), f(f) {}
        Move_only_type(const Move_only_type&) = delete;
        Move_only_type(Move_only_type&&) = default;
        Move_only_type& operator=(const Move_only_type&) = delete;
        Move_only_type& operator=(Move_only_type&&) = default;
        int i = 0;
        float f = 0.0f;        
    }; 
    
    bool operator==(const Move_only_type& lhs, const Move_only_type& rhs)
    {
        return lhs.i == rhs.i && lhs.f == rhs.f;
    }

    bool operator!=(const Move_only_type& lhs, const Move_only_type& rhs)
    {
        return lhs.i != rhs.i || lhs.f != rhs.f;
    }
    

    template <typename T,
			typename std::enable_if_t<std::is_copy_assignable<typename T::value_type>::value>* = nullptr>
    T& rv_fallback_cast(T& t) { return t; }
    
    template <typename T,
			typename std::enable_if_t<!std::is_copy_assignable<typename T::value_type>::value>* = nullptr>
    T&& rv_fallback_cast(T& t) { return std::move(t); }

    Simple_type& rv_fallback_cast(Simple_type& t) { return t; }

    Move_only_type&& rv_fallback_cast(Move_only_type& t) { return std::move(t); }

    template <typename T>
    void test_undo()
    {
        T value_1(3, 4);
        T value_2(2, 5);
        
        // Undo on nothing
        EXPECT_EQ(false, value_1.undo());
        EXPECT_EQ(false, value_1.has_save());
        // Single save and undo
        const auto value_1_old = rv_fallback_cast(value_1);
        EXPECT_EQ(true, value_1.save());
        EXPECT_EQ(true, value_1.has_save());
        value_1 = rv_fallback_cast(*value_2);
        EXPECT_NE(value_2, value_1_old);
        EXPECT_EQ(true, value_1.undo());
        EXPECT_EQ(false, value_1.has_save());
        EXPECT_EQ(value_1_old, value_1);

        // Double save
        EXPECT_EQ(true, value_1.save());
        value_1->i = 10;
        EXPECT_EQ(false, value_1.save());
        EXPECT_EQ(true, value_1.has_save());
        value_1 = rv_fallback_cast(*value_2);
        EXPECT_NE(value_2, value_1_old);
        EXPECT_EQ(true, value_1.undo());
        EXPECT_EQ(T(10, 4), value_1);
    }

    template <typename T>
    void test_swap()
    {
        T i_1 = 1;
        T i_2 = 2;
        const auto i_1_old = i_1;
        const auto i_2_old = i_2;

        using std::swap;
        swap(i_1, i_2);
        EXPECT_EQ(i_2_old, i_1);
        EXPECT_EQ(i_1_old, i_2);
        EXPECT_EQ(2, i_1);
        EXPECT_EQ(1, i_2);
    }

    template <typename T>
    void test_comparison()
    {
        T i_1 = 1;
        T i_2 = 2;
        T i_3 = 3;

        EXPECT_EQ(i_1, i_1);
        EXPECT_EQ(i_2, i_2);
        EXPECT_EQ(i_3, i_3);

        EXPECT_NE(i_1, i_2);
        EXPECT_NE(i_2, i_3);
        EXPECT_NE(i_3, i_1);

        EXPECT_LT(i_1, i_2);
        EXPECT_LT(i_2, i_3);

        EXPECT_LE(i_1, i_1);
        EXPECT_LE(i_1, i_2);
        EXPECT_LE(i_2, i_2);
        EXPECT_LE(i_2, i_3);
        EXPECT_LE(i_3, i_3);

        EXPECT_GT(i_2, i_1);
        EXPECT_GT(i_3, i_2);

        EXPECT_GE(i_1, i_1);
        EXPECT_GE(i_2, i_1);
        EXPECT_GE(i_2, i_2);
        EXPECT_GE(i_3, i_2);
        EXPECT_GE(i_3, i_3);
    }
}

TEST(HISTORY, UNDO)
{
    typedef undoable<Simple_type> Undo_simple_t;
    typedef undoable<Move_only_type> Undo_move_only_t;

    test_undo<Undo_simple_t>();
    test_undo<Undo_move_only_t>();
    
    // Test int
    typedef undoable<int> Undo_int_t;
    Undo_int_t i = 1;
    const Undo_int_t j = 5;
    
    EXPECT_EQ(1u, i.max_saves());
    EXPECT_EQ(0u, i.saves());
    const auto i_old = i;
    EXPECT_EQ(true, i.save());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(1u, i.saves());
    i = *j;
    EXPECT_EQ(j, i);
    EXPECT_EQ(true, i.undo());
    EXPECT_EQ(false, i.has_save());
    EXPECT_EQ(0u, i.saves());
    EXPECT_EQ(i_old, i);
}

TEST(HISTORY, UNDO_COMPARISONS)
{
    typedef undoable<int> Undo_int_t;
    Undo_int_t i_1 = 1;
    Undo_int_t i_2 = 2;
    Undo_int_t i_3 = 3;

    EXPECT_EQ(i_1, i_1);
    EXPECT_EQ(i_2, i_2);
    EXPECT_EQ(i_3, i_3);

    EXPECT_NE(i_1, i_2);
    EXPECT_NE(i_2, i_3);
    EXPECT_NE(i_3, i_1);

    EXPECT_LT(i_1, i_2);
    EXPECT_LT(i_2, i_3);

    EXPECT_LE(i_1, i_1);
    EXPECT_LE(i_1, i_2);
    EXPECT_LE(i_2, i_2);
    EXPECT_LE(i_2, i_3);
    EXPECT_LE(i_3, i_3);

    EXPECT_GT(i_2, i_1);
    EXPECT_GT(i_3, i_2);

    EXPECT_GE(i_1, i_1);
    EXPECT_GE(i_2, i_1);
    EXPECT_GE(i_2, i_2);
    EXPECT_GE(i_3, i_2);
    EXPECT_GE(i_3, i_3);
}

TEST(HISTORY, UNDO_SWAP)
{
    typedef undoable<int> Undo_int_t;
    test_swap<Undo_int_t>();
}

TEST(HISTORY, REDO)
{
    typedef redoable<Simple_type> Redo_simple_t;
    typedef redoable<Move_only_type> Redo_move_only_t;

    test_undo<Redo_simple_t>();
    test_undo<Redo_move_only_t>();

    // Test int
    typedef redoable<int> Redo_int_t;
    Redo_int_t i = 1;
    const Redo_int_t j = 5;

    // Test undo
    const auto i_old = i;
    EXPECT_EQ(true, i.save());
    EXPECT_EQ(1u, i.saves());
    EXPECT_EQ(true, i.has_save());
    i = *j;
    EXPECT_EQ(j, i);
    EXPECT_EQ(true, i.undo());
    EXPECT_EQ(false, i.has_save());
    EXPECT_EQ(i_old, i);

    // Test redo
    EXPECT_EQ(1u, i.max_edits());
    EXPECT_EQ(true, i.has_edit());
    EXPECT_EQ(1u, i.edits());
    EXPECT_EQ(true, i.redo());
    EXPECT_EQ(j, i);
    EXPECT_EQ(false, i.has_edit());
    EXPECT_EQ(0u, i.edits());
}

TEST(HISTORY, REDO_SWAP)
{
    typedef redoable<int> Redo_int_t;
    test_swap<Redo_int_t>();
}

TEST(HISTORY, REDO_COMPARISONS)
{
    typedef redoable<int> Redo_int_t;
    test_comparison<Redo_int_t>();
}

TEST(HISTORY, ARRAY_STORAGE)
{
	typedef redoable<int, array_storage<int, 3>> Redo_int_t;
	Redo_int_t i = 1;
	const Redo_int_t j = 5, k = 7, w = 10;

    // Test undo
	EXPECT_EQ(3u, i.max_saves());
    const auto i_old_0 = i;
    EXPECT_EQ(true, i.save());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(1u, i.saves());
    i = *j;
    EXPECT_EQ(j, i);

    const auto i_old_1 = i;
    EXPECT_EQ(true, i.save());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(2u, i.saves());
    i = *k;
    EXPECT_EQ(k, i);

    const auto i_old_2 = i;
    EXPECT_EQ(true, i.save());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(3u, i.saves());
    i = *w;
    EXPECT_EQ(w, i);
    const auto i_old_3 = i;

    EXPECT_EQ(true, i.undo());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(2u, i.saves());
    EXPECT_EQ(i_old_2, i);

    EXPECT_EQ(true, i.undo());
    EXPECT_EQ(true, i.has_save());
    EXPECT_EQ(1u, i.saves());
    EXPECT_EQ(i_old_1, i);

    EXPECT_EQ(true, i.undo());
    EXPECT_EQ(false, i.has_save());
    EXPECT_EQ(0u, i.saves());
    EXPECT_EQ(i_old_0, i);

    // Test redo
	EXPECT_EQ(3u, i.max_edits());

	EXPECT_EQ(true, i.has_edit());
	EXPECT_EQ(3u, i.edits());
	EXPECT_EQ(true, i.redo());
	EXPECT_EQ(i_old_1, i);

	EXPECT_EQ(true, i.has_edit());
	EXPECT_EQ(2u, i.edits());
	EXPECT_EQ(true, i.redo());
	EXPECT_EQ(i_old_2, i);

	EXPECT_EQ(true, i.has_edit());
	EXPECT_EQ(1u, i.edits());
	EXPECT_EQ(true, i.redo());
	EXPECT_EQ(i_old_3, i);

	EXPECT_EQ(false, i.has_edit());
	EXPECT_EQ(0u, i.edits());
}
