#pragma once

#include "../Definitions.hpp"

namespace Kedit {

struct Position {
	Nat row;
	Nat column;

	Position() = default;

	inline Position(Nat row, Nat column)
		: row(row), column(column) {}
};

}
