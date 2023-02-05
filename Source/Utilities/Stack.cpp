#include "Stack.hpp"

namespace Kedit {

template<class ObjectT>
Void Stack<ObjectT>::put(const ObjectT& object) noexcept {
	this->top_ = new Frame(object, this->top_);
	++this->height_;
}

template<class ObjectT>
Void Stack<ObjectT>::pop() {
	Frame* top = this->top_;
	delete this->top_;
	this->top_ = top;
	--this->height_;
}

}
