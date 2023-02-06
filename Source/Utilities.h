#pragma once
#ifndef KEDIT_UTILITIES_H
#define KEDIT_UTILITIES_H

#include <stdio.h>

#include "Types.h"

namespace Kedit {

class File {
public:
	File(const Sym* path, const Sym* modes) {
		if (!(this->_ptr = fopen(path, modes)))
			throw Error(0);
	}

	~File() {
		this->close();
	}

public:
	Void close() {
		if (fclose(this->_ptr) == EOF)
			throw Error(0);
	}


	Byte get() {
		Int c = fgetc(this->_ptr);
		if (c == EOF)
			throw Error(0);
		return c;
	}
	
private:
	FILE* _ptr;
};

class Output {
public:
	static Void write(Sym input) {
		if (fputc(input, stdout) == EOF)
			throw Error(0);
	}

	static Void write(const Sym* input) {
		if (fputs(input, stdout) == EOF)
			throw Error(0);
	}
	
	template<class... Args_T>
	static Void write(const Sym* input, Args_T... args) {
		if (fprintf(stdout, input, args...) == -1)
			throw Error(0);
	}
};

}

#endif
