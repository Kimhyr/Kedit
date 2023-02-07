#pragma once
#ifndef KEDIT_BUCKET_H
#define KEDIT_BUCKET_H

#include "Types.h"
#include "Utilities.h"

namespace Kedit {

template<class Water_T, Nat Capacity_T>
class Bucket {
public:
	Bucket() noexcept = default;
	Bucket(const Bucket& other) noexcept
		: _weight(other.weight()) {
		memcpy(this->_water, other.water(), other.weight());
	}
	Bucket(Bucket&&) = delete;

	Bucket& operator=(const Bucket& other) noexcept {
		this->_weight = other.weight();
		memcpy(this->_water, other.water(), other.weight());
		return *this;
	}
	Bucket& operator=(Bucket&&) = delete;

	~Bucket() = default;

public:
	constexpr const Water_T* water() const noexcept { return this->_water; }
	constexpr Nat weight() const noexcept { return this->_weight; }
	constexpr Nat capacity() const noexcept { return Capacity_T; }
	constexpr Bool empty() const noexcept { return this->weight() == 0; }
	constexpr Bool full() const noexcept { return this->weight() >= Capacity_T; }
	
	constexpr const Water_T* begin() const noexcept { return this->water(); }
	constexpr const Water_T* end() const noexcept { return this->water() + this->weight(); }

	constexpr Water_T& operator[](Int index) noexcept { return this->_water[index]; }
	constexpr Water_T& at(Nat index) noexcept {
		if (index >= this->mass())
			throw out_of_range("");
		return this->water()[index];
	}

	constexpr const Water_T& operator[](Int index) const noexcept { return this->_water[index]; }
	constexpr const Water_T& at(Nat index) const noexcept {
		if (index >= this->mass())
			throw out_of_range("");
		return this->water()[index];
	}

public:
	Bool operator==(const Bucket& other) const noexcept {
		if (other.weight() != this->weight())
			return false;
		for (auto& [i, j] : {this->_water, other._water}) {
			if (i != j)
				return false;
		}
		return true;
	}

	Void put(const Water_T& bit) noexcept {
		if (this->full())
			throw Error(0);
		*this->_end = bit;
		++this->_end;
	}

	Void pop() noexcept {
		if (this->empty())
			return;
		--this->_end;
		this->_end->~Water_T();
	}

	Void pop(Nat count = 1) noexcept {
		if (this->empty())
			return;
		do {
			--this->_end;
			this->_end->~Water_T();
		} while (--count && !this->empty());
	}

private:
	Water_T _water[Capacity_T];
	Nat _weight = 0;
};

}

#endif
