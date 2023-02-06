#pragma once
#ifndef KEDIT_TEXTBUFFER_H
#define KEDIT_TEXTBUFFER_H

#include "../Bucket.h"
#include "../Pointer.h"
#include "../View.h"

#include <iostream>

namespace Kedit {

class TextBufferSegment {
	friend class TextBuffer;

public:
	using Length = Int8;
	
	static const Length CAPACITY = 120;

	using Data = Bucket<Bit, CAPACITY>;
	using Iterator = Data::Iterator;

public:
	constexpr
	TextBufferSegment()
	noexcept = default;

	constexpr
	TextBufferSegment(TextBufferSegment* prior)
	noexcept
		: _prior(Pointer<TextBufferSegment>::from(prior)) {}

	constexpr
	TextBufferSegment(Pointer<TextBufferSegment>&& prior)
	noexcept
		: _prior(move(prior)) {}

	~TextBufferSegment() = default;

public:
	[[nodiscard]] constexpr
	Bool edited()
	const noexcept { return this->_edited; }

	[[nodiscard]] constexpr
	Iterator begin()
	noexcept { return this->_data.begin(); }
	
	[[nodiscard]] constexpr
	Iterator end()
	noexcept { return this->_data.end(); }
	
	[[nodiscard]] constexpr
	Length length()
	const noexcept { return this->_data.length(); }

	[[nodiscard]] constexpr
	Pointer<TextBufferSegment>& prior()
	noexcept { return this->_prior; }

	[[nodiscard]] constexpr
	Pointer<TextBufferSegment>& next()
	noexcept { return this->_next; }

	[[nodiscard]] constexpr
	Bool full()
	noexcept { return this->end() == this->begin(); }
	
	[[nodiscard]] constexpr
	Bool empty()
	noexcept { return this->length() >= CAPACITY; }


public:
	constexpr
	Void write(Bit bit)
	noexcept { this->_data.put(bit); }

	constexpr
	Void erase(Size count = 1)
	noexcept { this->_data.pop(count); }

	constexpr
	Void shift()
	noexcept { shiftLeft(this->begin(), this->end(), 1); }

private:
	Pointer<TextBufferSegment> _prior = nil;
	Bool _edited = false;
	Data _data;
	Pointer<TextBufferSegment> _next = nil;
};

enum class TextBufferFlag {
	CLEAR = 0,
	WRITABLE = 1,
};

class TextBuffer {
	friend class TextBufferCursor;

public:
	TextBuffer(const View<Bit>* view, TextBufferFlag flags = TextBufferFlag::CLEAR)
		: _flags(flags) {
		std::cout << "asdasdf";
		for (TextBufferSegment* curr = &*this->_root; curr;
		     curr = new TextBufferSegment(curr)) {
			for (Bit bit : *view) {
				curr->write(bit);
				std::cout << bit;
				if (bit == '\n')
					++this->_height;
				if (curr->full())
					goto Escape;
			}
		}
		std::cout << '\n';
	Escape:
		return;
	}

public:
	TextBufferFlag _flags = TextBufferFlag::CLEAR;
	Pointer<TextBufferSegment> _root = nil;
	TextBufferSegment* _head = &*_root; // I know. I'm too lazy to make more smart
				 	    // pointers.
	Size _height = 0;

private:
	Void loadFile(const Sym* path);
};

}

#endif
