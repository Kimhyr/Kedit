#include "Buffer.hpp"

namespace Kedit {

Void BufferCursor::write(Byte datum) noexcept {
	if (this->index_ == 0) {
		this->segment_.prepend(*new BufferSegment());
		this->segment_ = *this->segment_.prior();
	} else if () {
		
	} if (this->segment_.full()) {
		this->segment_ = *new BufferSegment(this->segment_);
		this->index_ = 0;
	} else if (this->index_ == 0) {
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

// TODO: Make this more performant.
Void BufferCursor::moveUp() {
	if (this->position_.row == 1)
		throw false;
	if (this->position_.column == 1)
		this->moveLeft();
	// "this->moveToLineStart()" will never throw becuase of the condition above.
	this->moveToLineStart();
	for (; this->position_.column != this->column_;
	     ++this->position_.column, ++this->index_) {
		if (this->segment_[this->index_] == '\n')
			break;
		if (this->index_ != this->segment_.mass())
			continue;
		this->segment_ = this->segment_.next();
		this->index_ = 0;
	}
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
	Nat column = 1;
	for (; segment[index] != '\n'; ++column) {
		if (!index) {
			if (!segment.prior())
				break;
			segment = segment.prior();
			index = segment.mass() - 1;
		} else --index;
	}
	return column;
}

Void BufferSegment::write(Byte datum) {
	if (this->mass_ == CAPACITY)
		throw false;
	this->data_[this->mass_] = datum;
	++this->mass_;
	this->edited_ = true;
}

Void BufferSegment::erase(Byte eraser) {
	if (this->mass_ == 0)
		throw false;
	--this->mass_;
	this->data_[this->mass_] = eraser;
	this->edited_ = true;
}

Void BufferSegment::split(Nat index) {
	if (index >= this->mass_)
		throw false;
	new BufferSegment(this);
	this->next_->fill(&this->data_[index], this->mass_ -= this->mass_ - index);
	this->edited_ = true;
}

Void BufferSegment::shift() {
	for (Nat i = 1; i < this->mass_; ++i)
		this->data_[i - 1] = this->data_[i];
	this->edited_ = true;
}

Void BufferSegment::fill(const Byte *data, Nat count) noexcept {
	for (this->mass_ = 0; this->mass_ < count; ++this->mass_)
		this->data_[this->mass_] = data[this->mass_];
	this->edited_ = true;
}

// TODO: Try doing without checking for "\n", but rather, base this on
// 	 "this->position_.column".
//	 I'm keeping this "TODO:" here because it looks clean.
Void BufferCursor::moveToLineStart() noexcept {
	do {
		if (this->index_)
			goto Continue;
		if (!this->segment_.prior())
			break;
		this->segment_ = this->segment_.prior();
		this->index_ = this->segment_.mass();
	Continue:
		--this->index_;
		--this->position_.column;
	} while (this->position_.column != 1);
}

}
