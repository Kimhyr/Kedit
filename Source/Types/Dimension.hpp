#pragma once

#include "../Definitions.hpp"

namespace Kedit {

struct Dimension {
	Nat width;
	Nat height;

	inline Dimension(Nat width, Nat height)
		: width(width), height(height) {}
};

}
