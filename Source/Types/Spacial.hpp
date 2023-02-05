#pragma once
#ifndef KEDIT_SPACIAL_HPP
#define KEDIT_SPACIAL_HPP

#include "Primitive.hpp"

namespace Kedit {

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
