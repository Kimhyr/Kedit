#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <assert.h>

#include "../Bucket.h"

namespace Kedit {

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
		using Bit = char;
		using Weight = nat16;
		static const Weight CAPACITY = 4; // TODO: Change this once done debugging.
		using Container = Bucket<Bit, CAPACITY>;
	
	public:
		Segment(bool edited)
			: _prior(nullptr), _edited(edited), _next(nullptr) {}
		Segment(Segment& prior, bool edited) noexcept;
		Segment(const Segment& other, bool edited = true) noexcept;
		Segment(Segment&&) = delete;
		
		Segment& operator=(const Segment& other) noexcept;
		Segment& operator=(Segment&&) = delete;

		~Segment() noexcept;
	
	public:
		constexpr bool edited() const noexcept { return this->_edited; }

		constexpr const Container& data() const noexcept { return this->_data; }

		constexpr const Bit* begin() const noexcept { return this->data().begin(); }
		constexpr const Bit* end() const noexcept { return this->data().end(); }

		constexpr Segment* prior() noexcept { return this->_prior; }
		constexpr Segment* next() noexcept { return this->_next; }
		
		constexpr Weight weight() const noexcept { return this->data().weight(); }
		constexpr bool full() const noexcept { return this->data().full(); }
		constexpr bool empty() const noexcept { return this->data().empty(); }

		constexpr const Bit& operator[](intptr index) const noexcept { return this->data()[index]; }
		constexpr const Bit& at(Weight index) const noexcept { return this->data().at(index); }
 
	public:
		constexpr void write(Bit input) { this->_data.put(input); }
		constexpr void erase(Weight count = 1) { this->_data.pop(count); }
		
		constexpr void shift(Weight count = 1) noexcept { std::shift_left(&this->_data[0], &this->_data[this->weight()], count); }

		void split(Weight at) noexcept;
		void prepend(Segment& prior);
	
		void print() const noexcept;
	
	private:
		Segment* _prior;
		bool _edited;
		Container _data;
		Segment* _next;

	private:
		void fill(Segment& from, const Bit* iter) noexcept;
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;

	public:
		using BitType = Segment::Bit;
		using WeightType = Segment::Weight;
		using ViewType = Segment::Container::View;
	
	public:
		struct State {
			natptr index;
			Segment& segment;
			const BitType* pointer;
			Position position;
			natptr column;
		};
	
	public:
		Cursor(TextBuffer& buffer) noexcept
			: _buffer(buffer), _index(0), _segment(buffer._root),
			  _pointer(this->_segment->begin() -1), _position(1, 1), _column(1) {}

		~Cursor() = default;

	public:
		constexpr const TextBuffer& buffer() const noexcept { return this->_buffer; } 
		constexpr const Segment* segment() const noexcept { return this->_segment; }

		constexpr const Position& position() const noexcept { return this->_position; }
		// TODO: Switch to using `natptr _index` rather `const BitType* _pointer`.
		constexpr natptr index() const noexcept { return this->_index; }
		constexpr natptr column() const noexcept { return this->_column; }

		constexpr const BitType* pointer() const noexcept { return this->_pointer; }
		constexpr BitType current() const noexcept { return *this->pointer(); }
		
		constexpr bool holding() const noexcept { return this->pointer() + 1 == this->segment()->begin(); }
		constexpr bool resting() const noexcept { return this->pointer() == this->segment()->begin(); }
		constexpr bool hanging() const noexcept { return this->pointer() + 1 == this->segment()->end(); }
		constexpr bool climbing() const noexcept { return this->pointer() >= this->segment()->begin() && this->pointer() < this->segment()->end(); }

	public:
		void write(const ViewType& view);

		void erase(natptr count = 1);

		void moveRight(natptr count = 1);
		void moveLeft(natptr count = 1);
		void moveUp(natptr count = 1);
		void moveDown(natptr count = 1);
		
		State locateBackwards(byte delimiter);
		State locateForwards(byte delimiter);

	private:
		TextBuffer& _buffer;
		natptr _index;
		Segment* _segment;
		const BitType* _pointer; // COMMENT: Maybe we can use `segment.weight - index + 1`.
		Position _position;
		natptr _column;

	private:
		natptr getColumn();
	
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
	constexpr Flag flags() const noexcept { return this->_flags; }
	constexpr const Segment* origin() const noexcept { return this->_origin; }
	constexpr natptr height() const noexcept { return this->_height; }
	constexpr Cursor& cursor() noexcept { return this->_cursor; }
	constexpr natptr weight() const noexcept { return this->_weight; }

public:
	void print() const noexcept;

private:
	Flag _flags;
	Segment* _origin;
        natptr _height;
	Cursor _cursor;
	natptr _weight;
};

using TextBufferFlag = TextBuffer::Flag;
using TextBufferSegment = TextBuffer::Segment;
using TextBufferCursor = TextBuffer::Cursor;

}
