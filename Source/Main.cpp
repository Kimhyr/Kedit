#include "Buffers/TextBuffer.h"

using namespace Kedit;

Int32 main() {
	Sym str[] = "penis_balls-cock";
	View<Bit> view(str, &str[sizeof(str)]);
	TextBuffer buffer(&view);
	buffer.print();
	return 0;
}
