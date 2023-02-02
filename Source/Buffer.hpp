#pragma once

#include "Types.hpp"

#include <stdio.h>

namespace Kedit {

class BufferSegment {
public:
	static constexpr const Size CAPACITY = 8;

public:
	inline BufferSegment()
		: prior_(nil), edited_(false), data_(), end_(this->data_),
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
	constexpr Bool empty() const noexcept { return this->end_ == this->data_; }

	inline Bit& operator [](Nat index) noexcept { return this->data_[index]; }

public:
	Void write(Bit bit);
	Void erase();

	Void shift();
	Void split(Bit* from);
	Void fill(BufferSegment& from, Bit* it) noexcept;
	
	Void prepend(BufferSegment& behind);

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
	inline BufferCursor(BufferSegment &segment)
		: segment_(segment), pointer_(segment.start()), position_(1, 1) {}

	~BufferCursor() = default;

public:
	inline const BufferSegment& segment() const noexcept { return this->segment_; }
	inline const Bit* pointer() const noexcept { return this->pointer_; }
	inline const Position& position() const noexcept { return this->position_; }
	inline Length column() const noexcept { return this->column_; }
	
	constexpr Bit current() const noexcept { return *this->pointer_; }
	constexpr Length index() const noexcept { return this->pointer_ - this->segment_.start(); }
	constexpr Bool sleeping() const noexcept { return this->pointer_ == this->segment_.start(); }
	constexpr Bool hanging() const noexcept { return this->pointer_ + 1 == this->segment_.end(); }

public:
	Void write(Bit bit = ' ') noexcept;
	Void erase() noexcept;

private:
	BufferSegment &segment_;
	const Bit* pointer_;
	Position position_;
	Length column_;

private:
	constexpr Void climb(const BufferSegment& segment) {
		this->segment_ = segment;
		this->pointer_ = this->segment_.start();
	}

	Void drop() noexcept;
};

class Buffer {
public:
	inline Buffer(const Sym* filePath)
		: root_(new BufferSegment()), cursor_(*this->root_), rows_(1) {
		this->loadFile(filePath);
	}

	~Buffer() noexcept;

public:
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

