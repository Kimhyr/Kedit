#pragma once
#ifndef KEDIT_FILE_HPP
#define KEDIT_FILE_HPP

#include "../Types.hpp"
#include "C.hpp"

namespace Kedit {

class File {
public:
	inline File(const Sym* path, const Sym* modes) {
		this->file_ = fopen(path, modes);
		if (!this->file_)
			throw false;
			// throw Error(Error::FOPEN);
	}

	inline ~File() noexcept {}

public:
	inline Void write(const Sym* text) {
		if (fputs(text, this->file_) == EOF)
			throw false;
			// throw Error(Error::FPUTS);
	}

public:
	inline Size size() const {
		if (fseek(this->file_, 0L, SEEK_END) != 0)
			throw false;
			// throw Error(Error::FSEEK);
		Int size = ftell(this->file_);
		if (size == -1L)
			throw false;
			// throw Error(Error::FTELL);
		if (fseek(this->file_, 0L, SEEK_SET) != 0)
			throw false;
			// throw Error(Error::FSEEK);
		return size;
	}

	constexpr Bool empty() const noexcept {
		return this->size() == 0;
	}

public:
	inline Byte get() {
		Int res = fgetc(this->file_);
		if (res == EOF)
			throw false;
			// throw Error(Error::FGETC);
		return res;
	}
	
	inline Void close() {
		if (fclose(this->file_) == EOF)
			throw false;
			// throw Error(Error::FCLOSE);
	}

private:
	FILE* file_;
};

}

#endif
