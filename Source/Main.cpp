#include "Buffer.hpp"

#include <iostream>

using namespace Kedit;

Int32 main() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1.txt");
	buffer.print();
	puts("==========================");
	// buffer.cursor().erase();
	buffer.cursor().write();
	buffer.print();
	return 0;
}
