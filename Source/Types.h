#pragma once

#include <functional>

namespace Kedit {

using I8 = signed char;
using I16 = signed short int;
using I32 = signed int;
using I64 = signed long int;

using N8 = unsigned char;
using N16 = unsigned short int;
using N32 = unsigned int;
using N64 = unsigned long int;

using R32 = float;
using R64 = double;

using B = bool;
using B8 =  N8;
using B16 = N16;
using B32 = N32;
using B64 = N64;

#if defined(__x86_64__)
	using I = I64;
	using N = N64;
	using R = R64;
#else
	using I = I32;
	using N = N32;
	using R = Rl32;
#endif

struct Position {
public:
	N row;
	N column;

public:
	constexpr Position() noexcept
		: row(0), column(0) {}
	explicit Position(N row, N column) noexcept
		: row(row), column(column) {}
};

}
