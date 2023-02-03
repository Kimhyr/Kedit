#include "Buffer.hpp"
#include "Error.hpp"

using namespace Kedit;

Int32 main() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1");
	buffer.print();
	puts("\n==========================");
	try {
		buffer.cursor().write('Z');
		buffer.cursor().write('Z');
		buffer.cursor().erase();
		buffer.print();
	} catch (ErrorCode c) {
		switch (c) {
		case ErrorCode::UNDERFLOW: puts("UNDERFLOW ERROR"); break;
		}
	}
	return 0;
}
