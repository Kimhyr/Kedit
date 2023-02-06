#pragma once
#ifndef KEDIT_ARRAY_H
#define KEDIT_ARRAY_H

#include "Types.h"

namespace Kedit {

template<class Bit_T, Size Length_T>
class Array {
public:
	using Iterator = Bit_T*;

public:
	constexpr
	Array()
	noexcept = default;
	
	Array(const Array& other) {
		for (auto& [i, j] : { *this, other })
			i = j;
		this->_end = this->_begin + other.length();
	}
	
	constexpr
	Array& operator=(const Array& other)
	noexcept {
		for (auto& [i, j] : { *this, other })
			i = j;
		this->_end = this->_begin + other.length();
	}

public:
	[[nodiscard]] constexpr
	Iterator begin()
	const noexcept { return this->_begin; }
	
	[[nodiscard]] constexpr
	Iterator end()
	const noexcept { return this->_end; }
	
	[[nodiscard]] constexpr
	Size length()
	const noexcept { return this->end() - this->begin(); }
	
	[[nodiscard]] constexpr
	Bit_T& at(Size index)
	const noexcept {
		if (index >= this->length())
			throw false;
		return this->_begin[index];
	}

	[[nodiscard]] constexpr
	Bit_T& operator[](Index index)
	noexcept { return this->_begin[index]; }

public:
	Bool operator==(const Array& other)
	const noexcept {
		if (other.length() != this->length())
			return false;
		for (auto& [bit1, bit2] : {other, *this}) {
			if (bit1 != bit2)
				return false;
		}
		return true;
	}

private:
	Bit_T _begin[Length_T];
	Iterator _end = &_begin[Length_T];
};

}

#endif
