// BUG: Erasing at the beginning of the buffer does not throw an exception,
// but, rather, it erases the following byte (the byte at the 0th index of the buffer).

#include "Buffers/TextBuffer.h"

using namespace Kedit;

void test0() {
	TextBuffer buf("/home/k/Projects/Kedit/Tests/Test1");
	std::string_view str("abcdef");
	buf.cursor().write(str);
	buf.cursor().erase(2);
	buf.cursor().write(str);
	buf.cursor().erase(10);
	buf.print();
	std::cout << std::endl;
}

void test1() {
	TextBuffer buf("/home/k/Projects/Kedit/Tests/Test1");
	bool running = true;
	std::string input;
	while (running) {
		std::cout << "\n>  ";
		std::cin >> input;
		if (input == "/") {
			buf.cursor().erase();
		} else if (input == "1") {
			buf.cursor().moveRight();
		} else if (input == "2") {
			buf.cursor().moveLeft();
		}
		else buf.cursor().write(input);
		buf.print();
	}
}

int32 main() {
	try {
		test1();
	} catch (const std::exception& e) {
		std::cout << "EXCEPTION: " << e.what() << '\n';
	}
	return 0;
}
