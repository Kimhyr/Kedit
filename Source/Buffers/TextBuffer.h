#pragma once
#ifndef KEDIT_TEXTBUFFER_H
#define KEDIT_TEXTBUFFER_H

#include "../Bucket.h"

namespace Kedit {

struct Position {
	length row;
	length column;

	explicit Position(length row, length column)
		: row(row), column(column) {}
};

class TextBuffer {
	friend class Segment;
	friend class Cursor;
	
public:
	enum class Flag: bool8 {
		CLEAR = 0,
		WRITABLE = 1,
	};

	class Segment {
		friend class TextBuffer;
		friend class Segment;
	
	public:
		using MassType = nat8;
		static const MassType CAPACITY = 4; // TODO: Change this once done debugging.
		using BucketType = Bucket<byte, CAPACITY>;
	
	public:
		constexpr Segment() noexcept = default;
		Segment(Segment& prior, bool edited) noexcept;
		Segment(const Segment& other) noexcept;
		Segment(Segment&&) = delete;
		
		Segment& operator=(const Segment& other) noexcept;
		Segment& operator=(Segment&&) = delete;

		~Segment() noexcept;
	
	public:
		constexpr bool edited() const noexcept { return this->_edited; }

		constexpr const BucketType& data() const noexcept { return this->_data; }

		constexpr const byte* begin() const noexcept { return this->data().begin(); }
		constexpr const byte* end() const noexcept { return this->data().end(); }

		constexpr unique_ptr<Segment>& prior() noexcept { return this->_prior; }
		constexpr unique_ptr<Segment>& next() noexcept { return this->_next; }
		
		constexpr MassType weight() const noexcept { return this->data().weight(); }
		constexpr bool full() const noexcept { return this->data().full(); }
		constexpr bool empty() const noexcept { return this->data().empty(); }

		constexpr const byte& operator[](index index) const noexcept { return this->data()[index]; }

	public:
		constexpr void write(byte byte) { this->_data.put(byte); }
		constexpr void erase(length count = 1) { this->_data.pop(count); }
		
		constexpr void shift(length count = 1) noexcept { shift_left(&this->_data[0], &this->_data[this->weight()], count); }

		void split(length at) noexcept;
		void prepend(Segment& prior, bool edited);
	
		void print() noexcept;
	
	private:
		unique_ptr<Segment> _prior;
		bool _edited = false;
		Bucket<byte, CAPACITY> _data;
		unique_ptr<Segment> _next = nullptr;


	private:
		void fill(Segment& from, const byte* iter) noexcept;
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;
	
	public:
		Cursor(Segment& segment) noexcept
			: _segment(segment), _position(1, 1), _column(1) {}

		~Cursor() = default;

	public:
		constexpr const Segment& segment() const noexcept { return this->_segment; }

		constexpr const Position& position() const noexcept { return this->_position; }
		constexpr length column() const noexcept { return this->_column; }

		constexpr ptr<const byte> pointer() const noexcept { return this->_pointer; }
		constexpr byte current() const noexcept { return *this->pointer(); }
		
		constexpr bool holding() const noexcept { return this->pointer() == this->segment().begin() - 1; }
		constexpr bool resting() const noexcept { return this->pointer() == this->segment().begin(); }
		constexpr bool hanging() const noexcept { return this->pointer() + 1 == this->segment().end(); }
		constexpr bool climbing() const noexcept { return this->pointer() >= this->segment().begin() && this->pointer() < this->segment().end(); }

	public:
		void write(byte input = ' ');
		void erase();

	private:
		Segment& _segment;
		byte* _pointer;
		Position _position;
		length _column;

	private:
		length getColumn();
	
	private:
		void climb(Segment& segment) noexcept;
		void drop(Segment& segment) noexcept;
		
		bool fall() noexcept;
		bool jump() noexcept;
	};

public:
	TextBuffer(const char* filePath, Flag flags = Flag::CLEAR);

	~TextBuffer();

public:
	void print() noexcept;

private:
	Flag _flags = Flag::CLEAR;
	unique_ptr<Segment> _root = make_unique<Segment>();
	unique_ptr<Segment> _head = make_unique<Segment>(*_root); // REFERENCE COUNTING TRIGGERS ME. I REFUSE TO USE shared_ptr.
	length _height = 0;
	Cursor _cursor;
};

}

#endif
