#include "BufferCursor.hpp"
#include "BufferSegment.hpp"

namespace Kedit {

Void BufferCursor::write(Byte datum) noexcept {
	if (this->segment_.full()) {
		this->segment_ = *new BufferSegment(this->segment_);
		this->index_ = 0;
	} else if (!this->atEndOfSegment())
		this->segment_.split(this->index_);
	this->segment_.write(datum);
	++this->index_;
	if (datum == '\n') {
		++this->position_.row;
		this->position_.column = 0;
	}
	++this->position_.column;
	this->column_ = this->position_.column;
}

Bool BufferCursor::erase(Byte eraser) {
	Bool nl = this->onNewLine();
	if (this->index_) {
		if (!this->atEndOfSegment())
			this->segment_.split(this->index_);
		this->segment_.erase(eraser);
		--this->index_;
	} else {
		if (!this->segment_.prior())
			throw false;
		this->segment_.erase(eraser);
		this->segment_.shift();
		this->segment_ = *this->segment_.prior();
		this->index_ = this->segment_.mass() - 1;
	}
	this->position_.row -= nl;
	if (nl)
		this->position_.column = this->getColumn();
	else --this->position_.column;
	this->column_ = this->position_.column;
	return nl;
}

Void BufferCursor::moveUp() {
	if (this->position_.row == 1)
		throw false;
	// Find the prior new line.
	// Go to the datum before the prior new line, and then start counting until the
	// prior prior new line.
	this->
}

Void BufferCursor::moveDown(Nat bufferHeight) {
	if (this->position_.row == bufferHeight)
		throw false;
}

Void BufferCursor::moveRight() {
	if (this->atEndOfSegment()) {
		if (!this->segment_.next())
			throw false;
		this->segment_ = this->segment_.next();
		this->index_ = 0;
	}
	if (this->onNewLine()) {
		++this->position_.row;
		this->position_.column = 0;
	}
	++this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::moveLeft() {
	Bool nl = this->onNewLine();
	if (!this->index_) {
		if (!this->segment_.prior())
			throw false;
		this->segment_ = this->segment_.prior();
		this->index_ = this->segment_.mass() - 1;
	}
	if (nl) {
		--this->position_.row;
		this->position_.column = this->getColumn();
	} else --this->position_.column;
	this->column_ = this->position_.column;
}

Nat BufferCursor::getColumn() const noexcept {
	BufferSegment &segment = this->segment_;
	Nat index = this->index_;
	Nat column;
	for (column = 1; segment[index] != '\n'; ++column) {
		if (!index) {
			if (!segment.prior())
				break;
			segment = segment.prior();
			index = segment.mass() - 1;
		} else --index;
	}
	return column;
}

}
