#pragma once

#include "Include.h"
#include "Range.h"

namespace Kedit {

template<class Water_T, length capacity_T>
class Bucket {
public:
	typedef basic_string_view<Water_T> ViewType;

public:
	Bucket() noexcept = default;
	Bucket(Bucket&&) = delete;
	Bucket(const Bucket& other) noexcept
		: _end(this->begin() + other.end()) {
		copy(other.begin(), other.end(), this->_water);
	}

	Bucket& operator=(Bucket&&) = delete;
	Bucket& operator=(const Bucket& other) noexcept {
		this->_weight = other.weight();
		copy(other.begin(), other.end(), this->_water);
		return *this;
	}

	~Bucket() = default;

public:
	constexpr const Water_T* water() const noexcept { return this->_water; }
	constexpr length capacity() const noexcept { return capacity_T; }

	constexpr const Water_T* begin() const noexcept { return this->water(); }
	constexpr const Water_T* end() const noexcept { return this->_end; }
	
	constexpr length weight() const noexcept { return this->end() - this->begin(); }
	constexpr bool empty() const noexcept { return this->begin() == this->end(); }
	constexpr bool full() const noexcept { return this->weight() >= this->capacity(); }

	constexpr Water_T& operator[](index index) noexcept { return this->_water[index]; }
	constexpr Water_T& at(length index) {
		if (index >= this->weight())
			throw out_of_range("");
		return this->water()[index];
	}

	constexpr const Water_T& operator[](index index) const noexcept { return this->_water[index]; }
	constexpr const Water_T& at(length index) const { 
		if (index >= this->weight())
			throw out_of_range("");
		return this->water()[index];
	}

public:
	inline bool operator==(const Bucket& other) const noexcept { return equal(this->begin(), this->end(), other.begin()); }

	void put(const ViewType& view) {
	}

	void put(Water_T&& bit) {
		if (this->full())
			throw out_of_range(__FUNCTION__);
		*this->_end = bit;
		++this->_end;
	}

	void put(const Water_T& bit) {
		if (this->full())
			throw out_of_range(__FUNCTION__);
		*this->_end = bit;
		++this->_end;
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
	Water_T _water[capacity_T];
	Water_T* _end;
};

}
