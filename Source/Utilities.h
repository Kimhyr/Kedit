#pragma once
#ifndef KEDIT_UTILITIES_H
#define KEDIT_UTILITIES_H

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

#include "Types.h"

namespace Kedit {

// This is only a bad practice if you are using other libraries,
// or you are creating a library.
using namespace std;

template<class T, class Length_T = Nat>
inline Bool cmpwlen(const T* first, Length_T firstLen, const T* second, Length_T secondLen) {
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
