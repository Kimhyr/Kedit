// An implementation of a "gapped doubly linked list of segments from some
// text". This data structure is split into 3 classes.
// 	* `BufferSegment` *segment*: A container for a part of a string. A
//	  segment may be empty, or partially empty (hence "gapped"), and only
//	  grows forwards, and dies backwards.
//	* `BufferCursor` *cursor*: A pointer to a bit inside of a segment. The
// 	  cursor can only point to a bit, never a gap. The cursor is used for
//	  traversing and modifying the buffer.
//	* `Buffer` *buffer*: A person that operates with segments and a cursor.
//
// ============================================================================

#pragma once
#ifndef KEDIT_BUFFER_HPP
#define KEDIT_BUFFER_HPP

#include "Types/Spacial.hpp"
#include "C.hpp"

namespace Kedit {

class BufferSegment {
public:
	// The amount of bits the segment can hold.
	static constexpr const Size CAPACITY = 4;

public:
	inline BufferSegment() noexcept
		: prior_(0), edited_(false), end_(this->data_),
		  next_(0) {}
	// Constructs the segment to have a prior segment. Useful for prepending a
	// segment with a non-nil prior segment.
	BufferSegment(BufferSegment& prior) noexcept;

	~BufferSegment() noexcept;

public:
	inline BufferSegment* prior() noexcept { return this->prior_; }
	inline Bool edited() const noexcept { return this->edited_; }
	inline const Bit* data() const noexcept { return this->data_; }
	inline const Bit* end() const noexcept { return this->end_; }
	inline BufferSegment* next() noexcept { return this->next_; }
	
	inline const Bit* start() const noexcept { return this->data_; }

	constexpr Size mass() const noexcept { return this->end_ - this->data_; }
	constexpr Bool full() const noexcept { return this->mass() == CAPACITY; }
	constexpr Bool empty() const noexcept { return this->end_ == this->start(); }

	inline Bit& operator [](Nat index) noexcept { return this->data_[index]; }

public:
	// Writes to the end of the segment. This method is wrapped by
	// `BufferCursor::write`.
	Void write(Bit bit);

	// Erases the last bit. This method is wrapped by `BufferCursor::erase`.
	Void erase();

	// Moves all the bits once to the left. Useful for erasing at the start of the
	// segment.
	Void shift();
	
	// Creates a new segment after the segment and moves the data from `from` to
	// the end of the segment into the new segment.
	Void split(Bit* from);

	// Fills the segment form another segment taking the bits from `it` to the end
	// of the given segment. This method is wrapped by `BufferSegment::split`.
	Void fill(BufferSegment& from, Bit* it) noexcept;

	// Attaches the given segment to the back of the segment.
	Void prepend(BufferSegment& prior);

	// Prints the segment's contents.
	Void print(); 

private:
	BufferSegment* prior_;
	Bool edited_;
	Bit data_[CAPACITY];
	Bit* end_;
	BufferSegment* next_;
};

class BufferCursor {
public:
	inline BufferCursor(BufferSegment& segment)
		: segment_(&segment), pointer_(segment.start()), position_(1, 1) {}

	~BufferCursor() = default;

public:
	// The segment that the cursor is on.
	inline const BufferSegment& segment() const noexcept { return *this->segment_; }

	// A pointer to the bit the cursor is on.
	inline const Bit* pointer() const noexcept { return this->pointer_; }

	// The position the cursor is on relative to the buffer.
	inline const Position& position() const noexcept { return this->position_; }
	
	// The column position the cursor is on. Used for trying to stay on the same
	// column when moving up or down.
	inline Length column() const noexcept { return this->column_; }

	// The current bit the cursor is on.
	constexpr Bit current() const noexcept { return *this->pointer_; }
	
	// The position the cursor is on relative to its segment.
	constexpr Length index() const noexcept { return this->pointer_ - this->segment_->start(); }
	
	// The cursor is at S[-1].
	constexpr Bool holding() const noexcept { return this->pointer_ == this->segment_->start() - 1; }
	
	// The cursor is at S[0].
	constexpr Bool resting() const noexcept { return this->pointer_ == this->segment_->start(); }

	// The cursor is at S[|S| - 1].
	constexpr Bool hanging() const noexcept { return this->pointer_ + 1 == this->segment_->end(); }

	// The cursor is at S[i > 0 & i < |S|];
	constexpr Bool climbing() const noexcept { return this->pointer_ >= this->segment_->start() && this->pointer_ < this->segment_->end(); }

public:
	// Writes a bit to where the cursor is pointed at.
	Void write(Bit bit = ' ') noexcept;

	// Erases the bit that cursor is on.
	Void erase();

private:
	BufferSegment* segment_;
	const Bit* pointer_;
	Position position_;
	Length column_;

private:
	// Moves the cursor to a next segment.
	Void climb(BufferSegment& segment) noexcept;

	// Moves the cursor to a prior segment.
	Void drop(BufferSegment& segment) noexcept;

	// Moves the cursor to a prior segment that is not empty.
	// Returns falls if the cursor did not fall.
	Bool fall() noexcept;

	// Moves the cursor to the next segment that is not empty.
	// Returns falls if the cursor did not jump.
	Bool jump() noexcept;
};

class Buffer {
public:
	Buffer(const Sym* filePath);

	~Buffer() noexcept;

public:
	inline BufferSegment* root() noexcept { return this->root_; }
	inline BufferCursor& cursor() noexcept { return this->cursor_; }
	inline Length rows() const noexcept { return this->rows_; }

public:
	Void print();

private:
	BufferSegment* root_;
	BufferCursor cursor_;
	Length rows_;

private:
	// Loads the contents of a file into linked segments.
	Void loadFile(const Sym *path);
};

}

#endif
