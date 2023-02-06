#pragma once
#ifndef KEDIT_TYPES_HPP
#define KEDIT_TYPES_HPP

#define nil nullptr

namespace Kedit {

using Nil = decltype(nil);

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
using Sym16 = char16_t;
using Sym32 = char32_t;

using Byte = unsigned char;
using Word = unsigned short int;
using DWord = unsigned int;
using QWord = unsigned int;

template<class Type_T>
using Ptr = Type_T*;

template<class Type_T>
using Ref = Type_T&;

#if defined(__x86_64__)
	using Int = Int64;
	using Nat = Nat64;
	using Real = Real64;
#else
	using Int = Int32;
	using Nat = Nat32;
	using Real = Real32;
#endif

using Error = Int;

enum class Ordering: Int8 {
	LESSER = -1,
	EQUAL = 0,
	GREATER = 1,
};

struct Position {
	Nat row;
	Nat column;

	constexpr Position(Nat row, Nat column)
		: row(row), column(column) {}
};

// template<class Element_T>
// class InitializerList {
// public:
// 	constexpr

	
// 	// constexpr
// 	// InitializerList()
// 	// noexcept
// 	// 	: _array(nil), _length(0) {}

// 	// constexpr
// 	// const Element_T* begin()
// 	// const noexcept { return this->_array; }
	
// 	// constexpr
// 	// const Element_T* end()
// 	// const noexcept { return this->begin() + this->_length; }

// 	// constexpr
// 	// Size length()
// 	// const noexcept { return this->_length; }

// private:
// 	// const Element_T* _array;
// 	// Size _length;

// private:
// 	// constexpr
// 	// InitializerList(const Element_T* arr, Size length)
// 	// 	: _array(arr), _length(length) {}
// };

}

#endif
