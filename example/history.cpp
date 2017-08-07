#include <mixme/wrap/history.hpp>
#include <iostream>
#include <string>

using namespace mixme::wrap;

struct String
{
	String(const std::string& s) : s(s) {}
	std::string s;
};

std::ostream& operator<<(std::ostream& os, const String& s)
{
	os << "Value = " << s.s;
	return os;
}

int main()
{
	// Start with a certain value
	redoable<String> test("my starting string");
	test.save();
	std::cout << *test << '\n';

	// Set to another value
	test = std::string("another string");
	std::cout << *test << '\n';

	// Undo edit
	test.undo();
	std::cout << *test << '\n';

	// Redo edit
	test.redo();
	std::cout << *test << '\n';
}
