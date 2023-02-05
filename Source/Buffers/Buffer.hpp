#pragma once
#ifndef KEDIT_BUFFER_HPP
#define KEDIT_BUFFER_HPP

#include "../Types.hpp"

namespace Kedit {

template<class Bit_T, class Container_T, class Out_T>
class Buffer {
public:
	virtual inline const Container_T& container() const noexcept = 0;

public:
	virtual Void write(Bit_T bit) = 0;

	virtual Void flush(Out_T& out) = 0;
};

}

#endif
