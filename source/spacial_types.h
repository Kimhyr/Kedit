#pragma once
#ifndef KEDIT_SPACIALTYPES_H
#define KEDIT_SPACIALTYPES_H

#include <cstddef>

namespace kedit {

struct position {
	size_t row;
	size_t column;

	constexpr position(size_t row, size_t column)
		: row(row), column(column) {}
};

}

#endif
