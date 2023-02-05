#pragma once
#ifndef KEDIT_BUFFER_HPP
#define KEDIT_BUFFER_HPP

#include "Utilities/C.hpp"
#include "Types/Spacial.hpp"

namespace Kedit {

class BufferSegment {
public:
	static constexpr const Size CAPACITY = 4;

public:
	inline BufferSegment() noexcept
		: prior_(nil), edited_(false), end_(this->data_),
		  next_(nil) {}
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
	Void write(Bit bit);
	Void erase();

	Void shift();
	Void split(Bit* from);

	Void fill(BufferSegment& from, Bit* it) noexcept;

	Void prepend(BufferSegment& prior);

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
		: segment_(&segment), pointer_(segment.start() - 1), position_(1, 1) {}

	~BufferCursor() = default;

public:
	inline const BufferSegment& segment() const noexcept { return *this->segment_; }
	inline const Bit* pointer() const noexcept { return this->pointer_; }
	inline const Position& position() const noexcept { return this->position_; }
	inline Length column() const noexcept { return this->column_; }
	
	constexpr Bit current() const noexcept { return *this->pointer_; }
	constexpr Length index() const noexcept { return this->pointer_ - this->segment_->start(); }
	constexpr Bool holding() const noexcept { return this->pointer_ == this->segment_->start() - 1; }
	constexpr Bool resting() const noexcept { return this->pointer_ == this->segment_->start(); }
	constexpr Bool hanging() const noexcept { return this->pointer_ + 1 == this->segment_->end(); }
	constexpr Bool climbing() const noexcept { return this->pointer_ >= this->segment_->start() && this->pointer_ < this->segment_->end(); }

public:
	Void write(Bit bit = ' ');
	Void erase();

private:
	BufferSegment* segment_;
	const Bit* pointer_;
	Position position_;
	Length column_;

private:
	Nat getColumn();

private:
	Void climb(BufferSegment& segment) noexcept;
	Void drop(BufferSegment& segment) noexcept;
	
	Bool fall() noexcept;
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
	Void loadFile(const Sym *path);
};

}

#endif
