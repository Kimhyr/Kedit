// For now, a basic error system.
//
// ============================================================================
// Errors are thrown enums. For example,
// 	if (sharted)
// 		throw ErrorCode::SHARTED;
// 
// ============================================================================
// An updated version of the error system will be implemented. Ideas on how the
// system are still being thought out. Some ideas include
// 	* a class with a constructor that prints to an error output stream; and
//	* a developmental error handler built for debugging.

#pragma once

namespace Kedit {

enum class ErrorCode {
	SUCCESS,

	FOPEN,
	FCLOSE,
	FPUTS,
	FSEEK,
	FTELL,
	FGETC,

	OVERFLOW,
	UNDERFLOW,
	OOR,
};

}
