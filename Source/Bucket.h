#pragma once
#ifndef KEDIT_BUCKET_H
#define KEDIT_BUCKET_H

#include "Include.h"

namespace Kedit {

template<class Water_T, nat Capacity_T>
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
	// WARNING: Maybe try using `unique_ptr`?
	constexpr const Water_T* water() const noexcept { return this->_water; }
	constexpr length weight() const noexcept { return this->_weight; }
	constexpr length capacity() const noexcept { return Capacity_T; }
	constexpr bool empty() const noexcept { return this->weight() == 0; }
	constexpr bool full() const noexcept { return this->weight() >= Capacity_T; }
	
	constexpr const Water_T* begin() const noexcept { return this->water(); }
	constexpr const Water_T* end() const noexcept { return this->water() + this->weight(); }

	constexpr Water_T& operator[](index index) noexcept { return this->_water[index]; }
	constexpr Water_T& at(length index) noexcept {
		if (index >= this->mass())
			throw out_of_range("");
		return this->water()[index];
	}

	constexpr const Water_T& operator[](index index) const noexcept { return this->_water[index]; }
	constexpr const Water_T& at(length index) const noexcept {
		if (index >= this->mass())
			throw out_of_range("");
		return this->water()[index];
	}

public:
	bool operator==(const Bucket& other) const noexcept {
		if (other.weight() != this->weight())
			return false;
		for (auto& [i, j] : {this->_water, other._water}) {
			if (i != j)
				return false;
		}
		return true;
	}

	bool put(const Water_T& bit) noexcept {
		if (this->full())
			throw out_of_range(__FUNCTION__);
		*this->_end = bit;
		++this->_end;
	}

	void pop() noexcept {
		if (this->empty())
			return;
		--this->_end;
		this->_end->~Water_T();
	}

	void pop(length count = 1) noexcept {
		if (this->empty())
			return;
		do {
			--this->_end;
			this->_end->~Water_T();
		} while (--count && !this->empty());
	}

private:
	Water_T _water[Capacity_T];
	length _weight = 0;
};

}

#endif
