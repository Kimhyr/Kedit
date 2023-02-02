#pragma once

#include "Types.hpp"
#include "Error.hpp"

#include <cstdio>
#include <iostream>

namespace Kedit {

class  IOMarker {
	
};

class File {
public:
	inline File(const Sym* path, const Sym* modes) {
		this->file_ = fopen(path, modes);
		if (!this->file_)
			throw ErrorCode::FOPEN;
	}

	inline ~File() noexcept {}

public:
	inline Void write(const Sym* text) {
		if (fputs(text, this->file_) == EOF)
			throw ErrorCode::FPUTS;
	}

public:
	inline Size size() const {
		if (fseek(this->file_, 0L, SEEK_END) != 0)
			throw ErrorCode::FSEEK;
		Int size = ftell(this->file_);
		if (size == -1L)
			throw ErrorCode::FTELL;
		if (fseek(this->file_, 0L, SEEK_SET) != 0)
			throw ErrorCode::FSEEK;
		return size;
	}

	constexpr Bool empty() const noexcept {
		return this->size() == 0;
	}

public:
	inline Byte get() {
		Int res = fgetc(this->file_);
		if (res == EOF)
			throw ErrorCode::FGETC;
		return res;
	}
	
	inline Void close() {
		if (fclose(this->file_) == EOF)
			throw ErrorCode::FCLOSE;
	}

private:
	FILE* file_;
};

}
