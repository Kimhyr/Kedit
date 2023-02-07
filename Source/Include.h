#pragma once
#ifndef KEDIT_UTILITIES_H
#define KEDIT_UTILITIES_H

#include <fstream>
#include <ranges>
#include <string_view>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <cstring>
#include <utility>
#include <algorithm>
#include <string>
#include <initializer_list>
#include <array>

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

using sym = char;
using sym8 = char;
using sym16 = char16_t;
using sym32 = char32_t;

using byte = unsigned char;
using word = unsigned short int;
using dword = unsigned int;
using qword = unsigned int;

template<class Type_T>
using ptr = Type_T*;

template<class Type_T>
using ref = Type_T&;

#if defined(__x86_64__)
	using index = int64;
	using length = nat64;
	using decimal = real64;
#else
	using index = int32;
	using length = nat32;
	using decimal = real32;
#endif

using intptr = index;
using natptr = length;

// This is only a bad practice if you are using other libraries,
// or you are creating a library.
using namespace std;

template<class T, class Length_T = length>
inline bool cmpwlen(const T* first, Length_T firstLen, const T* second, Length_T secondLen) {
	if (firstLen != secondLen)
		return false;
	for (auto& [i, j] : { first, second }) {
		if (i != j)
			return false;
	}
	return true;

}

}

#endif
