#pragma once

#include <string>

#include "Types.h"

namespace Kedit {

namespace ANSI {

constexpr const char* CSI = "\e[";

constexpr const char* RESET = "0";
constexpr const char* BOLD = "1";
constexpr const char* BRIGHT_RED = "90";
constexpr const char* BRIGHT_YELLOW = "93";
constexpr const char* BRIGHT_GREEN = "92";

enum Code {
	ATTR = 'm',
};

}

}
