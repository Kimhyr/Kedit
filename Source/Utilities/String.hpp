#pragma once
#ifndef KEDIT_STRING_HPP
#define KEDIT_STRING_HPP

#include "../Types.hpp"

namespace Kedit {

template<class BitT>
class String {
public:
	inline String(BitT* start, const BitT* end)
		: start_(start), end_(end) {}
	inline String(String& other)
		: start_(other.start_), end_(other.end_) {}
	String(const String& other);

	inline ~String() {
		delete[] this->start_;
	};

public:
	inline const BitT* data() const noexcept { return this->start_; }
	inline const BitT* start() const noexcept { return this->start_; }
	inline const BitT* end() const noexcept { return this->end_; }
	constexpr Length length() const noexcept { return this->end_ - this->start_; }
	
	constexpr const BitT& operator[](Length index) const noexcept { return this->start_[index]; }
	Bool operator==(const String& rhs) const noexcept;

public:
	const BitT& at(Length index) const;
	inline Bool compare(const String& other) const noexcept { return other == *this; }

private:
	BitT* start_;
	const BitT* end_;
};

using Text = String<Sym>;

}

#endif
