#include "Buffers/TextBuffer.h"

using namespace Kedit;

Int32 main() {
	Sym str[] = "1234567890";
	View<Bit> view(str, &str[sizeof(str)]);
	TextBuffer buffer(&view);
	return 0;
}
