#include "Error.hpp"

namespace Kedit {

Void printErrorCode(ErrorCode error) {
	switch (error) {
	ECP(SUCCESS);
	ECP(UNKNOWN_DECISION);
	ECP(FOPEN);
	ECP(FCLOSE);
	ECP(FPUTS);
	ECP(FSEEK);
	ECP(FTELL);
	ECP(FGETC);
	ECP(OVERFLOW);
	ECP(UNDERFLOW);
	ECP(OUT_OF_RANGE);
	default: std::cout << "Cuh, what?\n";
	}
}

}
