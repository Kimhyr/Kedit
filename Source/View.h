#pragma once
#ifndef KEDIT_VIEW_H
#define KEDIT_VIEW_H

#include "Types.h"

#include <initializer_list>

namespace Kedit {

template<class Bit_T = Bit, class Length_T = Size>
class View {
public:
	using Iterator = const Bit_T*;

public:
	View() = delete;
	
	explicit constexpr 
	View(Iterator begin, Iterator end)
	noexcept
		: _begin(begin), _end(end) {}

	constexpr
	View(const View& other) = delete;

	constexpr
	View(View&& other)
	noexcept 
		: _begin(other._begin), _end(other._end) {
		other._end = other._begin = nil;
	}

	constexpr
	View& operator=(const View& other) = delete;

	constexpr
	View& operator=(View&& other)
	noexcept {
		this->_begin = other._begin;
		this->_end = other._end;
		other._begin = nil;
		other._end = nil;
	}
	
public:
	[[nodiscard]] constexpr
	Iterator begin()
	const noexcept { return this->_begin; }
	
	[[nodiscard]] constexpr
	Iterator end()
	const noexcept { return this->_end; }
	
	[[nodiscard]] constexpr
	Length_T length()
	const noexcept { return this->end() - this->begin(); }
	
	[[nodiscard]] constexpr
	Bit& at(Length_T index)
	const noexcept;

	[[nodiscard]] constexpr
	Bit& operator[](Index index)
	const noexcept { return this->_begin[index]; }

public:
	Bool operator==(const View& other)
	const noexcept;

private:
	Iterator _begin;
	Iterator _end;
};

}

#endif
