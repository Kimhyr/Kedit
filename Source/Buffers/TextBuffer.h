#pragma once
#ifndef KEDIT_TEXTBUFFER_H
#define KEDIT_TEXTBUFFER_H

#include "../Bucket.h"
#include "../View.h"
#include "../Utilities.h"

#include <iostream>

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
		using MassType = Int8;
		static const MassType CAPACITY = 4;
	
	public:
		constexpr Segment() noexcept = default;
		Segment(Segment& prior) noexcept;

		Void prepend(Segment& prior);
	
		~Segment() noexcept;
	
	public:
		[[nodiscard]] constexpr Bool edited() const noexcept { return this->_edited; }
		
		constexpr const Bit* begin() const noexcept { return this->_data.begin(); }
		constexpr const Bit* end() const noexcept { return this->_data.end(); }

		[[nodiscard]] constexpr Segment* prior() noexcept { return this->_prior; }
		[[nodiscard]] constexpr Segment* next() noexcept { return this->_next; }
		
		constexpr MassType mass() const noexcept { return this->_data.mass(); }
		constexpr Bool full() const noexcept { return this->_data.full(); }
		constexpr Bool empty() const noexcept { return this->_data.empty(); }

// TODO: Iterators.

	public:
		constexpr Void write(Bit bit) noexcept { this->_data.put(bit); }
		constexpr Void erase(Nat count = 1) noexcept { this->_data.pop(count); }
		
		constexpr Void shift() noexcept { shiftLeft(this->_data.begin(), this->_data.end() + this->mass(), 1); }

		Void print() noexcept;
	
	private:
		Segment* _prior = nil;
		Bool _edited = false;
		Bucket<Bit, CAPACITY> _data;
		Segment* _next = nil;

	private:
		constexpr Bit* begin() noexcept { return this->_data.begin(); }
		constexpr Bit* end() noexcept { return this->_data.end(); }
	
	private:
		Void split(Bit* from) noexcept;
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
		[[nodiscard]] constexpr Segment* segment() const noexcept { return this->_segment; }

		[[nodiscard]] constexpr Position position() const noexcept { return this->_position; }
		[[nodiscard]] constexpr Nat column() const noexcept { return this->_column; }
		
		constexpr Bit current() const noexcept { return *this->_pointer; }
		
		constexpr Bool holding() const noexcept { return this->_pointer == this->_segment->begin() - 1; }
		constexpr Bit resting() const noexcept { return this->_pointer == this->_segment->begin(); }
		constexpr Bit hanging() const noexcept { return this->_pointer + 1 == this->_segment->end(); }
		constexpr Bit climbing() const noexcept { return this->_pointer >= this->_segment->begin() && this->_pointer < this->_segment->end(); }

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

	~TextBuffer();

public:
	Void print() noexcept;

private:
	Flag _flags = Flag::CLEAR;
	Segment* _root = new Segment;
	Segment* _head = _root;
	Nat _height = 0;
	Cursor _cursor;

private:
	Void loadFile(const Sym* path);
};

}

#endif
