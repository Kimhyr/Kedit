#pragma once

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
		typedef nat8 WeightType;
		static const WeightType CAPACITY = 4; // TODO: Change this once done debugging.
		typedef Bucket<byte, CAPACITY> ContainerType;
	
	public:
		Segment(bool edited = false)
			: _prior(nullptr), _edited(edited), _next(nullptr) {}
		Segment(Segment& prior, bool edited = true) noexcept;
		Segment(const Segment& other, bool edited = true) noexcept;
		Segment(Segment&&) = delete;
		
		Segment& operator=(const Segment& other) noexcept;
		Segment& operator=(Segment&&) = delete;

		~Segment() noexcept;
	
	public:
		constexpr bool edited() const noexcept { return this->_edited; }

		constexpr const ContainerType& data() const noexcept { return this->_data; }

		constexpr const byte* begin() const noexcept { return this->data().begin(); }
		constexpr const byte* end() const noexcept { return this->data().end(); }

		constexpr Segment* prior() noexcept { return this->_prior; }
		constexpr Segment* next() noexcept { return this->_next; }
		
		constexpr WeightType weight() const noexcept { return this->data().weight(); }
		constexpr bool full() const noexcept { return this->data().full(); }
		constexpr bool empty() const noexcept { return this->data().empty(); }

		constexpr const byte& operator[](index index) const noexcept { return this->data()[index]; }
		constexpr const byte& at(WeightType index) const noexcept { return this->data().at(index); }
 
	public:
		constexpr void write(byte input) { this->_data.put(input); }
		constexpr void erase(WeightType count = 1) { this->_data.pop(count); }
		
		constexpr void shift(WeightType count = 1) noexcept { shift_left(&this->_data[0], &this->_data[this->weight()], count); }

		void split(WeightType at) noexcept;
		void prepend(Segment& prior);
	
		void print() const noexcept;
	
	private:
		Segment* _prior;
		bool _edited;
		Bucket<byte, CAPACITY> _data;
		Segment* _next;

	private:
		void fill(Segment& from, const byte* iter) noexcept;
	};

	class Cursor {
		friend class TextBuffer;
		friend class Segment;

	public:
	
	public:
		Cursor(TextBuffer& buffer) noexcept
			: _buffer(buffer), _segment(buffer._root), _position(1, 1), _column(1) {}

		~Cursor() = default;

	public:
		constexpr const TextBuffer& buffer() const noexcept { return this->_buffer; } 
		constexpr const Segment* segment() const noexcept { return this->_segment; }

		constexpr const Position& position() const noexcept { return this->_position; }
		constexpr Segment::WeightType index() const noexcept { return this->pointer() - this->segment()->begin(); }
		constexpr length column() const noexcept { return this->_column; }

		constexpr const byte* pointer() const noexcept { return this->_pointer; }
		constexpr byte current() const noexcept { return *this->pointer(); }
		
		constexpr bool holding() const noexcept { return this->pointer() == this->segment()->begin() - 1; }
		constexpr bool resting() const noexcept { return this->pointer() == this->segment()->begin(); }
		constexpr bool hanging() const noexcept { return this->pointer() + 1 == this->segment()->end(); }
		constexpr bool climbing() const noexcept { return this->pointer() >= this->segment()->begin() && this->pointer() < this->segment()->end(); }

	public:
		void write(const Segment::ContainerType::ViewType& view);
		void write(byte input = ' ');

		void erase(Segment::WeightType count = 1);
		void erase();

	private:
		TextBuffer& _buffer;
		Segment* _segment;
		const byte* _pointer;
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
	constexpr Flag flags() const noexcept { return this->_flags; }
	constexpr const Segment* root() const noexcept { return this->_root; }
	constexpr const Segment* head() const noexcept { return this->_head; }
	constexpr length height() const noexcept { return this->_height; }
	constexpr Cursor& cursor() noexcept { return this->_cursor; }

public:
	void print() const noexcept;
	void forEach(void (*proc)(const Segment&)) const;

private:
	Flag _flags;
	Segment* _root = new Segment(false);
        Segment *_head = this->_root;
        length _height = 0;
	Cursor _cursor;
};

}
