#include "buffers/text_buffer.h"
#include <iostream>

using namespace kedit;

void test() {
	std::ifstream file("/home/k/Projects/kedit/tests/Test2");
	text_buffer buffer(file);
	file.close();
}

int main() {
	std::cout << "Hello, World!\n";
	test();
	return 0;
}
