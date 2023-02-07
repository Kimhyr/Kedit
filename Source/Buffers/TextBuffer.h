#pragma once
#ifndef KEDIT_TEXTBUFFER_H
#define KEDIT_TEXTBUFFER_H

#include "../Bucket.h"
#include "../View.h"

namespace Kedit {

class TextBuffer {
	friend class Segment;
	friend class Cursor;
	
public:
	enum class Flag {
		CLEAR = 0,
		WRITABLE = 1,
	};

	class Segment {
		friend class TextBuffer;
		friend class Segment;
	
	public:
		using MassType = Nat8;
		static const MassType CAPACITY = 4; // TODO: Change this once done debugging.
	
	public:
		constexpr Segment() noexcept = default;
		Segment(Segment& prior, Bool edited) noexcept;
		Segment(const Segment& other) noexcept;
		Segment(Segment&&) = delete;
		
		Segment& operator=(const Segment& other) noexcept;
		Segment& operator=(Segment&&) = delete;

		~Segment() noexcept;
	
	public:
		constexpr Bool edited() const noexcept { return this->_edited; }

		constexpr const Bucket<Bit, CAPACITY>& data() const noexcept { return this->_data; }

		constexpr const Bit* begin() const noexcept { return this->data().begin(); }
		constexpr const Bit* end() const noexcept { return this->data().end(); }

		constexpr Segment* prior() noexcept { return this->_prior; }
		constexpr Segment* next() noexcept { return this->_next; }
		
		constexpr MassType weight() const noexcept { return this->data().weight(); }
		constexpr Bool full() const noexcept { return this->data().full(); }
		constexpr Bool empty() const noexcept { return this->data().empty(); }

		constexpr const Bit& operator[](Int index) const noexcept { return this->data()[index]; }

	public:
		constexpr Void write(Bit bit) { this->_data.put(bit); }
		constexpr Void erase(Nat count = 1) { this->_data.pop(count); }
		
		Void shift(Nat count = 1) noexcept { shift_left(this->begin(), this->end(), count); }

		Void split(Nat at) noexcept;
		Void prepend(Segment& prior, Bool edited);
	
		Void print() noexcept;
	
	private:
		Segment* _prior = nil;
		Bool _edited = false;
		Bucket<Bit, CAPACITY> _data;
		Segment* _next = nil;

	private:
		constexpr Bit* begin() noexcept { return (Bit*)this->data().begin(); }
		constexpr Bit* end() noexcept { return (Bit*)this->data().end(); }
	
	private:
		Void fill(Segment& from, Bit* iter) noexcept;
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;
	
	public:
		constexpr Cursor(Segment& segment) noexcept
			: _segment(&segment), _position(1, 1), _column(1) {}

		~Cursor() = default;

	public:
		constexpr Segment* segment() const noexcept { return this->_segment; }

		constexpr Position position() const noexcept { return this->_position; }
		constexpr Nat column() const noexcept { return this->_column; }

		constexpr const Bit* pointer() const noexcept { return this->_pointer; }
		constexpr Bit current() const noexcept { return *this->pointer(); }
		
		constexpr Bool holding() const noexcept { return this->pointer() == this->segment()->begin() - 1; }
		constexpr Bit resting() const noexcept { return this->pointer() == this->segment()->begin(); }
		constexpr Bit hanging() const noexcept { return this->pointer() + 1 == this->segment()->end(); }
		constexpr Bit climbing() const noexcept { return this->pointer() >= this->segment()->begin() && this->pointer() < this->segment()->end(); }

	public:
		Void write(Bit bit = ' ');
		Void erase();

	private:
		Segment*_segment;
		Bit* _pointer;
		Position _position;
		Nat _column;

	private:
		Nat getColumn();
	
	private:
		Void climb(Segment& segment) noexcept;
		Void drop(Segment& segment) noexcept;
		
		Bool fall() noexcept;
		Bool jump() noexcept;
	};

public:
	TextBuffer(const View<Bit>* view, Flag flags = Flag::CLEAR) noexcept;
	TextBuffer(const Sym* filePath, Flag flags = Flag::CLEAR);

	~TextBuffer();

public:
	Void print() noexcept;

private:
	Flag _flags = Flag::CLEAR;
	Segment* _root = new Segment;
	Segment* _head = _root;
	Nat _height = 0;
	Cursor _cursor;
};

}

#endif
