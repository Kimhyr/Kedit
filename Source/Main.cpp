#include "Buffer.hpp"

using namespace Kedit;

Int32 main() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Buffer.cpp");
	buffer.print();
	printf("%lu", buffer.rows());
	return 0;
}
