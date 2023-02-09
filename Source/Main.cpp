// BUG: Erasing at the beginning of the buffer does not throw an exception,
// but, rather, it erases the following byte (the byte at the 0th index of the buffer).

#include "Buffers/TextBuffer.h"

using namespace Kedit;

int32 main() {
	try {
		TextBuffer buf("/home/k/Projects/Kedit/Tests/Test2");
		// std::string_view str("sddsf");
		buf.cursor().erase();
		// buf.print();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
