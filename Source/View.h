#pragma once
#ifndef KEDIT_VIEW_H
#define KEDIT_VIEW_H

#include "Types.h"
#include "Utilities.h"

namespace Kedit {

template<class Bit_T = Bit, class Length_T = Nat>
class View {
public:
	View() = delete;
	
	explicit View(const Bit_T* begin, const Bit_T* end) noexcept
		: _begin(begin), _end(end) {}

	View(const View& other) = delete;

	View(View&& other) noexcept
		: _begin(other._begin), _end(other._end) {
		other->_end = other->_begin = nil;
	}

	View& operator=(const View& other) = delete;

	View& operator=(View&& other) noexcept {
		if (this == &other)
			return;
		this->_begin = other->_begin;
		this->_end = other->_end;
		other->_begin = nil;
		other->_end = nil;
	}
	
public:
	constexpr const Bit_T* begin() const noexcept { return this->_begin; }
	constexpr const Bit_T* end() const noexcept { return this->_end; }
	constexpr Length_T length() const noexcept { return this->end() - this->begin(); }

	constexpr const Bit& operator[](Int index) const noexcept { return this->_begin[index]; }
	constexpr const Bit& at(Length_T index) const noexcept {
		if (index >= this->length())
			throw out_of_range("");
		return this->begin()[index];
	}

public:
	inline Bool operator==(const View& other) const noexcept {
		return cmpwlen(this->_begin, this->length(), other._begin, other.length());
	}

private:
	const Bit_T* _begin;
	const Bit_T* _end;
};

}

#endif
