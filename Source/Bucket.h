#pragma once
#ifndef KEDIT_BUCKET_H
#define KEDIT_BUCKET_H

#include "Utilities.h"

namespace Kedit {

template<class Bit_T, Size Capacity_T>
class Bucket {
public:
	using Iterator = Bit_T*;

public:
	constexpr
	Bucket()
	noexcept = default;
	
	Bucket(const Bucket& other)
	noexcept {
		for (auto& [i, j] : { *this, other })
			i = j;
		this->_end = this->_begin + other.length();
	}

	constexpr
	Bucket& operator=(const Bucket& other)
	noexcept {
		for (auto& [i, j] : { *this, other })
			i = j;
		this->_end = this->_begin + other.length();
	}

public:
	[[nodiscard]] constexpr
	Iterator begin()
	noexcept { return this->_begin; }

	[[nodiscard]] constexpr
	Iterator end()
	noexcept { return this->_end; }

	[[nodiscard]] constexpr
	Size length()
	const noexcept { return this->_end - this->_begin; }
	
	[[nodiscard]] constexpr
	Size capacity()
	const noexcept { return Capacity_T; }

	[[nodiscard]] constexpr
	Bool empty()
	const noexcept { return this->_end == this->_begin; }
	
	[[nodiscard]] constexpr
	Bool full()
	const noexcept { return this->length() == this->capacity(); }

	[[nodiscard]] constexpr
	const Bit_T& at(Size index)
	const noexcept {
		if (index >= this->length())
			throw false;
		return this->_begin[index];
	}

	[[nodiscard]] constexpr
	const Bit_T& operator[](Index index)
	noexcept { return this->_begin[index]; }

public:
	Bool operator==(const Bucket& other)
	const noexcept {
		if (other.length() != this->length())
			return false;
		for (auto& [bit1, bit2] : {other, *this}) {
			if (bit1 != bit2)
				return false;
		}
		return true;
	}

	Void put(const Bit_T& bit) {
		if (this->full())
			throw Error(0);
		*this->_end = bit;
		++this->_end;
	}

	Void put(Bit_T&& bit) {
		if (this->full())
			throw Error(0);
		*this->_end = bit;
		++this->_end;
	}

	Void pop() {
		if (this->empty())
			throw Error(0);
		--this->_end;
		this->_end->~Bit_T();
	}

	Void pop(Size count = 1) {
		if (this->empty())
			throw Error(0);
		do {
			--this->_end;
			this->_end->~Bit_T();
		} while (!this->empty() && --count);
	}

private:
	Bit_T _begin[Capacity_T];
	Iterator _end = _begin;
};

}

#endif
