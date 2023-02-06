#pragma once
#ifndef KEDIT_UTILITIES_H
#define KEDIT_UTILITIES_H

#include <stdio.h>

#include "Types.h"

namespace Kedit {

class File {
public:
	File(const Sym* path, const Sym* modes) {
		if (nil == (this->_ptr = fopen(path, modes)))
			throw Error(0);
	}

	~File() {
		this->close();
	}

public:
	Bool empty() {
		Int64 last = fseek(this->_ptr, 0, SEEK_CUR);
		if (last == EOF)
			throw Error(0);
		if (fseek(this->_ptr, 0, SEEK_END) != 0)
			throw Error(0);
		Int64 res = ftell(this->_ptr);
		if (res == EOF)
			throw Error(0);
		if (fseek(this->_ptr, last, SEEK_SET) != 0)
			throw Error(0);
		return res == 0;
	}
	
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

	Void put(Byte byte) {
		if (fputc(byte, this->_ptr) != byte)
			throw Error(0); 
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
