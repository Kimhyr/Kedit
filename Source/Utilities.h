#pragma once
#ifndef KEDIT_UTILITIES_H
#define KEDIT_UTILITIES_H

#include <algorithm>

#include "Types.h"

namespace Kedit {

template<class Type_T>
struct RemoveRef { using Type = Type_T; };

template<class Type_T>
struct RemoveRef<Type_T&> { using Type = Type_T; };

template<class Type_T>
struct RemoveRef<Type_T&&> { using Type = Type_T; };

template<class Type_T> [[nodiscard]] constexpr
Type_T&& forward(typename RemoveRef<Type_T>::Type& value)
noexcept { return static_cast<Type_T&&>(value); }

template<class Type_T> [[nodiscard]] constexpr
Type_T&& forward(typename RemoveRef<Type_T>::Type&& value)
noexcept { return static_cast<Type_T&&>(value); }

template<class Type_T> [[nodiscard]] constexpr
typename RemoveRef<Type_T>::Type&& move(Type_T&& value)
noexcept { return static_cast<typename RemoveRef<Type_T>::Type&&>(value); }

template<class Iterator_T, class Difference_T = Nat> constexpr
Iterator_T shiftLeft(Iterator_T first, Iterator_T last, Nat count = 1) {
	return std::shift_left(first, last, count); // Lol
	// if (count == 0)
	// 	return last;
	// auto mid = next(first, count, last);
	// if (mid == last)
	// 	return first;
	// return move(move(mid), move(last), move(first));
}

}

#endif
