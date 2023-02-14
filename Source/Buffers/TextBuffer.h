#pragma once

#include "../Bucket.h"

namespace Kedit {

class TextBuffer {
	friend class Segment;
	friend class Cursor;

public:
	class Segment {
		friend class TextBuffer;
		friend class Cursor;
	public:
		using Bit = char;
		using Weight = nat16;
		static const Weight CAPACITY = 4;
		using Container = Bucket<Bit, CAPACITY>;

	public:
		constexpr Segment(bool edited) noexcept 
			: _prior(this), _edited(edited), _next(this) {}
		constexpr Segment(Segment& prior, bool edited) noexcept
			: _prior(&prior), _edited(edited), _next(prior._next) {
			prior._next->_prior = this;
			prior._next = this;
		}
			
		Segment(const Segment&) = delete;
		Segment(Segment&&) = delete;

		Segment& operator=(const Segment&) = delete;
		Segment& operator=(Segment&&) = delete;

		~Segment() noexcept;

	public:
		constexpr Segment* prior() noexcept { return this->_prior; }
		constexpr const Segment* prior() const noexcept { return this->_prior; }

		constexpr bool edited() const noexcept { return this->_edited; }

		constexpr const Container& data() const noexcept { return this->_data; }

		constexpr Segment* next() noexcept { return this->_next;  }
		constexpr const Segment* next() const noexcept { return this->_next;  }

		constexpr const Bit* begin() const noexcept { return this->data().begin(); }
		constexpr const Bit* end() const noexcept { return this->data().end(); }

		constexpr Weight weight() const noexcept { return this->data().weight(); }
		constexpr bool full() const noexcept { return this->data().full(); }
		constexpr bool empty() const noexcept { return this->data().empty(); }

		constexpr const Bit& operator[](intptr index) const noexcept { return this->data()[index]; }
		constexpr const Bit& at(Weight index) const noexcept { return this->data().at(index); }

	public:
		constexpr void write(Bit input) { return this->_data.put(input); }
		constexpr void erase(Weight count = 1) { return this->_data.pop(count); }

		constexpr void shift(Weight count = 1) noexcept { return this->_data.shift(count); }

		void split(Weight at) noexcept;
		constexpr void prepend(Segment& prior) noexcept {
			prior._prior = this->_prior;
			prior._next = this;
			prior._prior->_next
		}

		void print() const noexcept;
	
	private:
		Segment* _prior;
		bool _edited;
		Container _data;
		Segment* _next;

	private:
		void fill(Segment& from, const Bit* iter);
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;

	public:
		using Bit = Segment::Bit;
		using Weight = Segment::Weight;
		using View = Segment::Container::View;

	public:
		struct State {
			natptr index;
			Segment& segment;
			const Bit* pointer;
			Position position;
			natptr column;
		};

	public:
		Cursor(TextBuffer& buffer) noexcept
			: _buffer(buffer), _segment(buffer._root), _index(0),
			  _pointer(this->_segment->begin() - 1), _position(1, 1), _column(1) {}

		~Cursor() = default;

	public:
		constexpr const TextBuffer& buffer() const noexcept { return this->_buffer; }
		constexpr const Segment* segment() const noexcept { return this->_segment; }

		constexpr natptr index() const noexcept { return this->_index; }
		constexpr const Bit* pointer() const noexcept { return this->_pointer; }
		constexpr const Position& position() const noexcept { return this->_position; }
		constexpr natptr column() const noexcept { return this->_column; }

		constexpr Weight depth() const noexcept { return this->pointer() - this->segment()->begin(); }
		constexpr Bit current() const noexcept { return *this->pointer(); }

		constexpr bool holding() const noexcept { return this->pointer() + 1 == this->segment()->begin(); }
		constexpr bool resting() const noexcept { return this->pointer() == this->segment()->begin(); }
		constexpr bool hanging() const noexcept { return this->pointer() + 1 == this->segment()->end(); }
		constexpr bool climbing() const noexcept { return this->pointer() >= this->segment()->begin() && this->pointer() < this->segment()->end(); }
	
	public:
		void write(const View& view);
		void erase(natptr count = 1);

		void moveRight(natptr count = 1);
		void moveLeft(natptr count = 1);
		void moveUp(natptr count = 1);
		void moveDown(natptr count = 1);

		State locateBackwards(byte delimiter);
		State locateForwards(byte delimiter);
	
	private:
		TextBuffer& _buffer;
		Segment* _segment;
		natptr _index;
		const Bit* _pointer;
		Position _position;
		natptr _column;

	private:
		void climb(Segment& to) noexcept;
		void drop(Segment& to) noexcept;

		bool fall() noexcept;
		bool jump() noexcept;
	};

	enum class Flag {
		CLEAR = 0,
		WRITABLE = 1,
	};

public:
	TextBuffer(const char* filePath, Flag flags = Flag::CLEAR);

	~TextBuffer();

public:
	constexpr Flag flags() const noexcept { return this->_flags; }
	constexpr const Segment* root() const noexcept { return this->_root; }
	constexpr natptr height() const noexcept { return this->_height; }
	constexpr natptr weight() const noexcept { return this->_weight; }
	constexpr Cursor& cursor() noexcept { return this->_cursor; }

public:
	void print() const noexcept;

private:
	Flag _flags;
	Segment* _root;
	natptr _weight;
	natptr _height;
	Cursor _cursor;
};

using TextBufferSegment = TextBuffer::Segment;
using TextBufferCursor = TextBuffer::Cursor;
using TextBufferFlag = TextBuffer::Flag;

}
