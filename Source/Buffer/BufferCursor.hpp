#pragma once

#include "../Types/Position.hpp"
#include "BufferSegment.hpp"

namespace Kedit {

class BufferCursor {
public:
	inline BufferCursor(BufferSegment &segment)
		: segment_(segment), index_(0), position_(1, 1), column_(1) {}

	~BufferCursor() = default;

public:
	inline const BufferSegment &segment() const noexcept { return this->segment_; }
	inline Nat index() const noexcept { return this->index_; }
	inline const Position &position() const noexcept { return this->position_; }
	inline Nat column() const noexcept { return this->column_; }

public:
	constexpr Bool atEndOfSegment() const noexcept { return this->index_ + 1 == this->segment_.mass(); }
	constexpr Bool onNewLine() const noexcept { return this->segment_[this->index_] == '\n'; }

public:
	Void write(Byte datum) noexcept;

	// Throws "false" if "!this->segment_.prior()".
	// Returns true if erased a "\n".
	Bool erase(Byte eraser = ' ');

	Void moveUp();
	
	Void moveDown(Nat lastRow);
	
	Void moveRight();
	
	Void moveLeft();

public:
	// TODO: Experiment O(log(n)) ~> O(1).
	// 	* For each segment, there exists a list of "Line"s containing:
	// 		- start: Index, and
	//		- column: Nat.
	Nat getColumn() const noexcept;

private:
	BufferSegment &segment_;
	Nat index_;
	Position position_;
	Nat column_;
};

}
