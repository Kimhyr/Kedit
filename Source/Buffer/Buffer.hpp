// Tasks:
// 	* Handle height changes due to erasing.

#pragma once

#include "BufferCursor.hpp"

namespace Kedit {

class Buffer {

private:
	Nat lines_;
	BufferSegment *root_;
	BufferCursor cursor_;
};

}

