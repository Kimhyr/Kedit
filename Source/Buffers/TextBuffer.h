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
		using BitType = char;
		using WeightType = nat16;
		static const WeightType CAPACITY = 4; // TODO: Change this once done debugging.
		using BodyType = Bucket<BitType, CAPACITY>;
	
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

		constexpr const BodyType& data() const noexcept { return this->_data; }

		constexpr const BitType* begin() const noexcept { return this->data().begin(); }
		constexpr const BitType* end() const noexcept { return this->data().end(); }

		constexpr Segment* prior() noexcept { return this->_prior; }
		constexpr Segment* next() noexcept { return this->_next; }
		
		constexpr WeightType weight() const noexcept { return this->data().weight(); }
		constexpr bool full() const noexcept { return this->data().full(); }
		constexpr bool empty() const noexcept { return this->data().empty(); }

		constexpr const BitType& operator[](intptr index) const noexcept { return this->data()[index]; }
		constexpr const BitType& at(WeightType index) const noexcept { return this->data().at(index); }
 
	public:
		constexpr void write(BitType input) { this->_data.put(input); }
		constexpr void erase(WeightType count = 1) { this->_data.pop(count); }
		
		constexpr void shift(WeightType count = 1) noexcept { std::shift_left(&this->_data[0], &this->_data[this->weight()], count); }

		void split(WeightType at) noexcept;
		void prepend(Segment& prior);
	
		void print() const noexcept;
	
	private:
		Segment* _prior;
		bool _edited;
		BodyType _data;
		Segment* _next;

	private:
		void fill(Segment& from, const BitType* iter) noexcept;
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;

	public:
		using BitType = Segment::BitType;
		using WeightType = Segment::WeightType;
		using ViewType = Segment::BodyType::ViewType;
	
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
		constexpr WeightType index() const noexcept { return this->pointer() - this->segment()->begin(); }
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
	constexpr const Segment* root() const noexcept { return this->_root; }
	constexpr const Segment* head() const noexcept { return this->_head; }
	constexpr natptr height() const noexcept { return this->_height; }
	constexpr Cursor& cursor() noexcept { return this->_cursor; }
	constexpr natptr weight() const noexcept { return this->_weight; }

public:
	void print() const noexcept;

private:
	Flag _flags;
	Segment* _root;
        Segment* _head;
        natptr _height;
	Cursor _cursor;
	natptr _weight;
};

using TextBufferFlag = TextBuffer::Flag;
using TextBufferSegment = TextBuffer::Segment;
using TextBufferCursor = TextBuffer::Cursor;

}
