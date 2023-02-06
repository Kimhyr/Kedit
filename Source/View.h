#pragma once
#ifndef KEDIT_VIEW_H
#define KEDIT_VIEW_H

#include "Types.h"

namespace Kedit {

template<class Bit_T = Bit, class Length_T = Nat>
class View {
public:
	View() = delete;
	
	explicit constexpr View(const Bit_T* begin, const Bit_T* end) noexcept
		: _begin(begin), _end(end) {}

	constexpr View(const View& other) = delete;

	constexpr View(View* other) noexcept
		: _begin(other->_begin), _end(other->_end) {
		other->_end = other->_begin = nil;
	}

	constexpr View& operator=(const View& other) = delete;

	constexpr View& operator=(View* other) noexcept {
		this->_begin = other->_begin;
		this->_end = other->_end;
		other->_begin = nil;
		other->_end = nil;
	}
	
public:
	[[nodiscard]] constexpr const Bit_T* begin() const noexcept { return this->_begin; }
	[[nodiscard]] constexpr const Bit_T* end() const noexcept { return this->_end; }
	constexpr Length_T length() const noexcept { return this->end() - this->begin(); }

	constexpr Bit& at(Length_T index) const noexcept {
		if (index >= this->length())
			throw Error(0);
		return this->_begin[index];
	}
	constexpr Bit& operator[](Int index) noexcept { return this->_begin[index]; }

	constexpr const Bit& operator[](Int index) const noexcept { return this->_begin[index]; }

public:
	Bool operator==(const View& other)
	const noexcept;

private:
	const Bit_T* _begin;
	const Bit_T* _end;
};

}

#endif
