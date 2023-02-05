#include "String.hpp"

namespace Kedit {

template<class BitT>
String<BitT>::String(const String& other)
	: start_(new BitT[other.length()]), end_(this->start_ + other.length()) {
	for (Length i = 0; i < other.length(); ++i)
		this->start_[i] = other.start_[i];
}

template<class BitT>
Bool String<BitT>::operator==(const String& other) const noexcept {
	if (other != this->length())
		return false;
	for (Length i = 0; i < this->length(); ++i)
		if (other.start()[i] != this->start_[i])
			return false;
	return true;
}

template<class BitT>
const BitT& String<BitT>::at(Length index) const {
	if (index >= this->length())
		throw false;
	return (*this)[index];
}

}
