#pragma once
#ifndef KEDIT_POINTER_H
#define KEDIT_POINTER_H

#include <memory>

#include "Types.h"
#include "Utilities.h"

namespace Kedit {

template<class Type_T>
class Pointer {
public:
	constexpr
	Pointer()
	noexcept
		: _ptr(nil) {}
	
	constexpr
	Pointer(Nil)
	noexcept
		: _ptr(nil) {}

	explicit constexpr
	Pointer(Type_T* ptr)
	noexcept
		: _ptr(ptr) {}

	constexpr
	Pointer(Pointer&& other)
	noexcept
		: _ptr(other._ptr) { other._ptr = nil; }

	constexpr
	Pointer& operator=(Pointer&& other)
	noexcept {
		this->_ptr = other._ptr;
		other._ptr = nil;
	}

	constexpr
	Pointer& operator=(Nil)
	noexcept { this->_ptr = nil; }

	template<class... Args_T> static constexpr
	Pointer from(Args_T&&... args)
	noexcept { return Pointer(new Type_T(forward(args)...)); }

	static constexpr
	Pointer from(Type_T* ptr)
	noexcept { return Pointer(new Type_T(ptr)); }
	
	constexpr
	~Pointer()
	noexcept { delete this->_ptr; }

public:
	constexpr
	Type_T& operator*()
	const noexcept { return *this->_ptr; }

	constexpr
	Type_T* operator->()
	const noexcept { return this->_ptr; }

	constexpr
	Ordering operator<=>(const Pointer& other)
	const noexcept { return this->_ptr <=> other._ptr; }
	
	constexpr
	Bool operator>(const Pointer& other)
	const noexcept { return this->_ptr > other._ptr; }
	
	constexpr
	Bool operator<(const Pointer& other)
	const noexcept { return this->_ptr < other._ptr; }
	
	constexpr
	Bool operator==(const Pointer& other)
	const noexcept { return this->_ptr == other._ptr; }

	constexpr
	Bool operator!=(const Pointer& other)
	const noexcept { return this->_ptr != other._ptr; }

	constexpr
	Type_T& operator[](Index index)
	const noexcept { return this->_ptr[index]; }

private:
	Type_T* _ptr;
};

}

#endif
