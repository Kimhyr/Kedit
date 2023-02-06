#include "Buffers/TextBuffer.h"
#include "Utilities.h"

using namespace Kedit;

Void fromView() {
	Sym str[] = "Hello, World!";
	View<Bit> view(str, &str[sizeof(str)]);
	TextBuffer buffer(&view);
	buffer.print();
}

Void fromFile() {
	TextBuffer buffer("/home/k/Projects/Kedit/Tests/Test3");
	buffer.print();
}

Int32 main() {
	fromFile();
	return 0;
}
