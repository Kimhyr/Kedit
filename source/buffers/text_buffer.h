#pragma once
#ifndef KEDIT_TEXTBUFFER_H
#define KEDIT_TEXTBUFFER_H

#include <array>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>

#include "../spacial_types.h"

namespace kedit {

using bit = char;

class text_buffer_segment {
	friend class text_buffer;

public:
	static
	const int capacity = 120;

	using data_type = std::array<bit, capacity>;
	using size_type = data_type::size_type;
	using difference_type = std::array<bit, capacity>::difference_type;
	using iterator = data_type::iterator;

	constexpr
	text_buffer_segment(bool edited = false)
		: _edited(edited) {}

	[[nodiscard]] constexpr
	bool edited() const noexcept
	{ return this->_edited; }
	
	[[nodiscard]] constexpr
	const bit* data() const noexcept
	{ return this->_data.data(); }
	
	[[nodiscard]] constexpr
	size_type mass() const noexcept
	{ return this->_mass; }
	
	[[nodiscard]] constexpr
	bool full() const noexcept
	{ return this->mass() == this->_data.max_size(); }

	[[nodiscard]] constexpr
	bool empty() const noexcept
	{ return this->mass() == 0; }

	[[nodiscard]] constexpr
	bit* begin() noexcept
	{ return this->_data.begin(); }

	[[nodiscard]] constexpr
	bit* end() noexcept
	{ return this->_data.end(); }

	[[nodiscard]] constexpr
	bit& operator[](size_t index) noexcept
	{ return this->_data[index]; }

	[[nodiscard]] constexpr
	bit& at(size_t index)
	{ return this->_data.at(index); }

	void write(bit bit);
	void write(const std::basic_string<bit>& bits);

	void erase(size_type count = 0);

	constexpr
	void shift() noexcept
	{ std::shift_left(this->begin(), this->end(), 1); }

private:
	bool _edited;
	std::array<bit, capacity> _data;
	size_type _mass;
};

class text_buffer_cursor {
	friend class text_buffer;

public:
	using list = std::list<text_buffer_segment>;
	using iterator = list::iterator;
	
	text_buffer_cursor(iterator&& it)
		: _segment(it), _location(1, 1), _column(1) {}

	[[nodiscard]] constexpr
	const iterator& segment() const noexcept
	{ return this->_segment; }

	[[nodiscard]] constexpr
	size_t index() const noexcept
	{ return this->_index; }

	[[nodiscard]] constexpr
	bit current() const noexcept
	{ return (*this->_segment)[_index]; }

	[[nodiscard]] constexpr
	const position& location() const noexcept
	{ return this->_location; }
	
	[[nodiscard]] constexpr
	size_t column() const noexcept
	{ return this->_column; }

	[[nodiscard]] constexpr
	bool holding() const noexcept
	{ return this->_index == 0; }

	[[nodiscard]] constexpr
	bool resting() const noexcept
	{ return this->_index == 1; }

	[[nodiscard]] constexpr
	bool hanging() const noexcept
	{ return this->_index + 1 == this->_segment->mass(); }

	[[nodiscard]] constexpr
	bool climbing() const noexcept
	{ return this->_index > 0 && this->_index < this->_segment->mass(); }

	void write(bit bit = ' ');

	void erase();

private:
	iterator& _segment;
	size_t _index;
	position _location;
	size_t _column;
};

enum class text_buffer_flag {
	writable = 1,
};

class text_buffer {
	using list_type = std::list<text_buffer_segment>;

	text_buffer(std::ifstream& file);

	[[nodiscard]] constexpr
	const text_buffer_cursor& cursor() const noexcept
	{ return this->_cursor; }

	[[nodiscard]] constexpr
	size_t height() const noexcept
	{ return this->_height; }

private:
	list_type _segments;
	text_buffer_cursor _cursor;
	size_t _height = 1;
};

}

#endif
