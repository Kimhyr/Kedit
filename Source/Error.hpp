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
