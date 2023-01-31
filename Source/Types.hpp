#pragma once

#include "Definitions.hpp"

namespace Kedit {

struct Position {
	Nat row;
	Nat column;

	Position() = default;

	inline Position(Nat row, Nat column)
		: row(row), column(column) {}
};

struct Dimension {
	Nat width;
	Nat height;

	inline Dimension(Nat width, Nat height)
		: width(width), height(height) {}
};

struct Rect {
	Position position;
	Dimension dimension;

	inline Rect(const Position &position, const Dimension &dimension)
		: position(position), dimension(dimension) {}
};

}
