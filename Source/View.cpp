#include "View.h"

namespace Kedit {

template<class Bit_T, class Length_T> constexpr
Bit& View<Bit_T, Length_T>::at(Length_T index)
const noexcept {
	if (index >= this->length())
		throw false;
	return this->_begin[index];
}

template<class Bit_T, class Length_T>
Bool View<Bit_T, Length_T>::operator==(Ref<const View> other)
const noexcept {
	if (other.length() != this->length())
		return false;
	for (auto& [i, j] : {*this, other}) {
		if (i != j)
			return false;
	}
	return true;
}

}
