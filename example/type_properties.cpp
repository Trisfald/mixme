#include <mixme/gift/type_properties.hpp>
#include <iostream>
#include <utility>

using namespace mixme::gift;

struct Base
{
	Base() = default;
	Base(const Base&) { std::cout << "copy constructor" << std::endl; }
	Base(Base&&) { std::cout << "move constructor" << std::endl; }
	Base& operator=(const Base&) { std::cout << "copy assignment" << std::endl; return *this; }
	Base& operator=(Base&&) { std::cout << "move assignment" << std::endl; return *this; }
};

struct Copy : Base, copy_only {};
struct Move : Base, move_only {};
struct Nothing : Base, no_copy_or_move {};

int main()
{
	// Only copy operations are allowed
	Copy a;
	auto b = a;
	a = b;
	auto c = std::move(a);
	a = std::move(c);

	// Only move operations are allowed
	Move d;
	auto e = std::move(d);
	d = std::move(e);
}
