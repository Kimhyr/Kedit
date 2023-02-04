// For now, a basic error system.
//
// ============================================================================
// Errors are thrown enumerants. For example,
// 	if (sharted)
// 		throw ErrorCode::SHARTED;
// 
// ============================================================================
// An updated version of the error system will be implemented. Ideas on how the
// system are still being thought out. Some ideas include
// 	* a class with a constructor that prints to an error output stream; and
//	* a developmental error handler built for debugging.

#pragma once
#ifndef KEDIT_ERROR_HPP
#define KEDIT_ERROR_HPP

#include "Types/Primitive.hpp"
#include "C.hpp"

namespace Kedit {

enum class ErrorCode: Nat8 {
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

#define ECP(ENUM) case ErrorCode::ENUM: std::cout << #ENUM "\n"; break

Void printErrorCode(ErrorCode error);

}

#endif
