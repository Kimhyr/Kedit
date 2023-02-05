#pragma once
#ifndef KEDIT_TYPES_HPP
#define KEDIT_TYPES_HPP

#define nil nullptr

namespace Kedit {

using Bit = char;

using Void = void;

using Int8 = signed char;
using Int16 = signed short int;
using Int32 = signed int;
using Int64 = signed long int;

using Nat8 = unsigned char;
using Nat16 = unsigned short int;
using Nat32 = unsigned int;
using Nat64 = unsigned long int;

using Real32 = float;
using Real64 = double;

using Bool = Nat8;
using Bool8 = Nat8;
using Bool16 = Nat16;
using Bool32 = Nat32;
using Bool64 = Nat64;

using Sym = char;
using Sym8 = char;
using Smy16 = char16_t;
using Smy32 = char32_t;

using Byte = unsigned char;
using Word = unsigned short int;
using DWord = unsigned int;
using QWord = unsigned int;

using Addr = const Void *; 

#if defined(__x86_64__)
  using Int = Int64;
  using Nat = Nat64;
  using Real = Real64;
#else
  using Int = Int32;
  using Nat = Nat32;
  using Real = Real32;
#endif

using Length = Nat;
using Size = Nat;

struct Position {
	Length row;
	Length column;

	Position() = default;

	inline Position(Nat row, Nat column)
		: row(row), column(column) {}
};

struct Dimension {
	Length width;
	Length height;

	inline Dimension(Nat width, Nat height)
		: width(width), height(height) {}
};

struct Rectangle {
	Position position;
	Dimension dimension;

	inline Rectangle(const Position &position, const Dimension &dimension)
		: position(position), dimension(dimension) {}
};

}

#endif
