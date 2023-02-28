#pragma once

#include <array>
#include <list>
#include <string_view>

#include "Types.h"

namespace Kedit {

class String_Buffer {
public:
	using This_Type = String_Buffer;
	using Weight_Type = N;
	using Difference_Type = Weight_Type;
	using Bit_Type = N8;

private:
	struct Segment {
		using This_Type = String_Buffer;
		using Weight_Type = N;
		using Bit_Type = String_Buffer::Bit_Type;
	
	public:
		static constexpr N const CAPACITY = 1024;
	
	public:
		std::array<Bit_Type, CAPACITY> data;
		Bit_Type const* end;
	};

public:
	constexpr std::list<Segment> const& segments() const noexcept { return this->segments_; }
	constexpr Segment const* current_segment() const noexcept { return this->current_segment_; }
	constexpr Bit_Type const* cursor() const noexcept { return this->cursor_; }

public:
	void write(std::string_view const& string);

	void erase(Difference_Type difference);

private:
	std::list<Segment> segments_;
	Segment* current_segment_;
	B8 const* cursor_;
};

}
