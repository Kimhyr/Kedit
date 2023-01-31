// Tasks:
// 	* Handle height changes due to erasing.

#pragma once

#include "Types.hpp"

namespace Kedit {

class BufferSegment {
public:
	static constexpr const Nat CAPACITY = 80;

public:
	inline BufferSegment()
		: edited_(false), mass_(0), prior_(nil), next_(nil) {}

	inline BufferSegment(BufferSegment *prior)
		: edited_(false), mass_(0), prior_(prior) {
		this->next_ = prior->next_;
		prior->next_ = this;
	}

	inline BufferSegment(BufferSegment &prior)
		: edited_(false), mass_(0), prior_(&prior) {
		this->next_ = prior.next_;
		prior.next_ = this;
	}

	inline ~BufferSegment() {
		this->prior_->next_ = this->next_;
		this->next_->prior_ = this->prior_;
	}
public:
	inline Bool edited() const noexcept { return this->edited_; }
	inline const Byte *data() const noexcept { return this->data_; }
	inline Nat mass() const noexcept { return this->mass_; }
	inline BufferSegment *prior() noexcept { return this->prior_; }
	inline BufferSegment *next() noexcept { return this->next_; }
	inline Byte &operator [](Nat index) noexcept { return this->data_[index]; }

public:
	constexpr Bool full() const noexcept { return this->mass() == CAPACITY; }

public:
	Void write(Byte datum);

	Void erase(Byte eraser);

	Void split(Nat index);

	Void shift();

protected:
	Void fill(const Byte *data, Nat count) noexcept;

private:
	Bool edited_;
	Byte data_[CAPACITY];
	Nat mass_;
	BufferSegment *prior_;
	BufferSegment *next_;
};

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

private:
	Void moveToLineStart() noexcept;
};

class Buffer {

private:
	Nat lines_;
	BufferSegment *root_;
	BufferCursor cursor_;
};

}

