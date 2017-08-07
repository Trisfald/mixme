#include <mixme/gift/comparison.hpp>
#include <iostream>

using namespace mixme;

// We have a struct with operator > defined
struct my_struct
{
	my_struct() = default;
	my_struct(int i) : i(i) {}
	int i = 0;
};

// Give to a struct operator <
struct my_struct_lt : my_struct, gift::comparison::lt<my_struct_lt>
{
	using my_struct::my_struct;
};

bool operator>(const my_struct_lt& lhs, const my_struct_lt& rhs) { return lhs.i > rhs.i; }

// Give to a struct operator !=
struct my_struct_ne : my_struct, gift::comparison::ne<my_struct_ne>
{
	using my_struct::my_struct;
};

bool operator>(const my_struct_ne& lhs, const my_struct_ne& rhs) { return lhs.i > rhs.i; }

// Give to a struct all operators
struct my_struct_all : my_struct, gift::comparison::all<my_struct_all>
{
	using my_struct::my_struct;
};

bool operator>(const my_struct_all& lhs, const my_struct_all& rhs) { return lhs.i > rhs.i; }


template <typename F>
void utility_printer(const std::string& msg, F f)
{
	std::cout << msg << ": " << std::boolalpha << f() << '\n';
}


int main()
{
	// Try the gifted less than operator
	my_struct_lt lt_1(1), lt_4(4), lt_7(7);
	utility_printer("1 < 4", [=]{ return lt_1 < lt_4; });
	utility_printer("7 < 1", [=]{ return lt_7 < lt_1; });
	utility_printer("4 < 4", [=]{ return lt_4 < lt_4; });
	std::cout << '\n';

	// Try the gifted not equal operator
	my_struct_ne ne_1(1), ne_4(4), ne_7(7);
	utility_printer("1 != 4", [=]{ return ne_1 != ne_4; });
	utility_printer("7 != 1", [=]{ return ne_7 != ne_1; });
	utility_printer("4 != 4", [=]{ return ne_4 != ne_4; });
	std::cout << '\n';

	// Try some gifted operators
	my_struct_all all_1(1), all_4(4), all_7(7);
	utility_printer("1 == 4", [=]{ return all_1 == all_4; });
	utility_printer("1 == 1", [=]{ return all_1 == all_1; });
	utility_printer("7 <= 4", [=]{ return all_7 <= all_4; });
	utility_printer("1 <= 7", [=]{ return all_1 <= all_7; });
	utility_printer("4 >= 4", [=]{ return all_4 >= all_4; });
	utility_printer("1 >= 7", [=]{ return all_1 >= all_7; });
}
