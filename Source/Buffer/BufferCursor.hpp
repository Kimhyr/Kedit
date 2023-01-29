#pragma once

#include "../Types/Position.hpp"
#include "BufferSegment.hpp"

namespace Kedit {

class BufferCursor {
public:
	inline BufferCursor(BufferSegment &segment)
		: segment_(segment), index_(0), position_(1, 1) {}

	~BufferCursor() = default;

public:
	Void write(Byte datum);
	
	Void erase(Byte eraser = ' ');

	Void moveUp();
	
	Void moveDown();
	
	Void moveRight();
	
	Void moveLeft();

private:
	BufferSegment &segment_;
	Nat index_;
	Position position_;
};

}
