#include "BufferCursor.hpp"

namespace Kedit {

Void BufferCursor::write(Byte datum) {
	// TODO
}

Void BufferCursor::erase(Byte eraser) {
	Bool nl = this->segment_[this->index_] == '\n';
	if (this->index_ == 0) {
		if (!this->segment_.prior())
			throw false;
		this->segment_.prior()->erase(eraser);
		this->segment_ = *this->segment_.prior();
		this->index_ = this->segment_.mass() - 1;
	} else if (this->index_ + 1 == this->segment_.mass()) {
		this->segment_.erase(eraser);
		--this->index_;
	} else {
		if (this->segment_.split(this->index_)) {
			this->segment_ = *this->segment_.next();
			this->index_ = 0;
		} else --this->index_;
	}
	this->position_.row -= nl;
	this->position_.column = this->calculateColumn();
}

Void BufferCursor::moveUp() {
	
}

Void BufferCursor::moveDown() {
	
}

Void BufferCursor::moveRight() {
	
}

Void BufferCursor::moveLeft() {
	
}



}
