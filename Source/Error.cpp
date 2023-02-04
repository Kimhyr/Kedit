#include "Error.hpp"

#define ECP(ENUM) case Error::ENUM: std::cout << #ENUM "\n"; break

namespace Kedit {

Error::Error(Code code)
	: code_(code) {
	switch (code) {
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
