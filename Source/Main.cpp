#include "Buffer.hpp"

#include "Error.hpp"

using namespace Kedit;

constexpr Bool alphabetic(Sym input) {
	return (input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z');
}

constexpr Bool numeric(Sym input) {
	return (input >= '0' && input <= '9');
}

Void test0() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1");
	Sym input = 0;
	while (true) {
		std::cout << "\n\n>  ";
		std::cin >> input;
		if (input == '/')
			buffer.cursor().erase();
		else if (alphabetic(input) || numeric(input))
			buffer.cursor().write(input);
		buffer.print();
	}
}

Void test1() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1");
	buffer.print();
	puts("\n==========================");
	buffer.cursor().write('Z');
	buffer.cursor().write('Z');
	buffer.cursor().erase();
	buffer.print();
}

Int32 main() {
	try {
		test0();
	} catch (Error) {
		return 1;
	}
	return 0;
}
