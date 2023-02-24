#pragma once

#include <functional>

namespace Kedit {

using int8 = signed char;
using int16 = signed short int;
using int32 = signed int;
using int64 = signed long int;

using nat = unsigned int;
using nat8 = unsigned char;
using nat16 = unsigned short int;
using nat32 = unsigned int;
using nat64 = unsigned long int;

using real = double;
using real32 = float;
using real64 = double;

using bool8 = nat8;
using bool16 = nat16;
using bool32 = nat32;
using bool64 = nat64;

using char8 = char;
using char16 = char16_t;
using char32 = char32_t;

using byte = unsigned char;
using word = unsigned short int;
using dword = unsigned int;
using qword = unsigned int;

#if defined(__x86_64__)
	using intptr = int64;
	using natptr = nat64;
	using realptr = real64;
#else
	using intptr = int32;
	using natptr = nat32;
	using realptr = real32;
#endif

struct Position {
public:
	natptr row;
	natptr column;

public:
	constexpr Position() noexcept
		: row(0), column(0) {}
	explicit Position(natptr row, natptr column) noexcept
		: row(row), column(column) {}
};

}
