#include "DoublyLink.hpp"

namespace Kedit {

template<class Data_T>
DoublyLink<Data_T>::~DoublyLink() noexcept {
	for (Node* node; this->root_ != nil; this->root_ = node) {
		node = this->root_;
		delete this->root_;
	}
}

}
