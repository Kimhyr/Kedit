#pragma once
#ifndef KEDIT_BUCKET_H
#define KEDIT_BUCKET_H

#include "Types.h"

namespace Kedit {

template<class Bit_T, Nat Capacity_T>
class Bucket {
public:
	constexpr Bucket() noexcept = default;
	
	Bucket(const Bucket& other) noexcept {
		for (auto& [i, j] : { *this, *other })
			i = j;
		this->_end = this->_begin + other.mass();
	}

	Bucket(Bucket* other) noexcept
		: _begin(other->_begin), _end(this->_begin + other->mass()){
		other->_begin = nil;
		other->_end = nil;
	}

public:
	[[nodiscard]] constexpr const Bit_T* begin() const noexcept { return this->_begin; }
	[[nodiscard]] constexpr const Bit_T* end() const noexcept { return this->_end; }
	[[nodiscard]] constexpr Bit_T* begin() noexcept { return this->_begin; }
	[[nodiscard]] constexpr Bit_T* end() noexcept { return this->_end; }
	
	constexpr Nat mass() const noexcept { return this->_end - this->_begin; }
	[[nodiscard]] constexpr Nat capacity() const noexcept { return Capacity_T; }
	constexpr Bool empty() const noexcept { return this->_end == this->_begin; }
	constexpr Bool full() const noexcept { return this->mass() == this->capacity(); }
	
	constexpr const Bit_T& at(Nat index) const noexcept {
		if (index >= this->mass())
			throw false;
		return this->_begin[index];
	}
	constexpr const Bit_T& operator[](Int index) noexcept { return this->_begin[index]; }

public:
	Bool operator==(const Bucket& other) const noexcept {
		if (other.mass() != this->mass())
			return false;
		for (auto& [bit1, bit2] : {other, *this}) {
			if (bit1 != bit2)
				return false; }
		return true;
	}

	Void put(const Bit_T& bit) {
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

	Void pop(Nat count = 1) {
		if (this->empty())
			throw Error(0);
		do {
			--this->_end;
			this->_end->~Bit_T();
		} while (!this->empty() && --count);
	}

private:
	Bit_T _begin[Capacity_T];
	Bit_T* _end = _begin;
};

}

#endif
