#pragma once
#ifndef KEDIT_ERROR_HPP
#define KEDIT_ERROR_HPP

#include "Types/Primitive.hpp"
#include "Utilities/C.hpp"

namespace Kedit {

class Error {
public:
	enum Code: Nat8 {
		SUCCESS,
		UNKNOWN_DECISION,

		FOPEN,
		FCLOSE,
		FPUTS,
		FSEEK,
		FTELL,
		FGETC,

		OVERFLOW,
		UNDERFLOW,
		OUT_OF_RANGE,
	};

public:
	Error(Code code);

	~Error() = default;

public:
	inline Code code() const noexcept { return this->code_; }

private:
	Code code_;
};

class ErrorHandler {
};

}

#endif
