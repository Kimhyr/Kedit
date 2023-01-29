#include "BufferSegment.hpp"

namespace Kedit {

Void BufferSegment::write(Byte datum) {
	if (this->mass_ == CAPACITY)
		throw false;
	this->data_[this->mass_] = datum;
	++this->mass_;
	this->edited_ = true;
}

Void BufferSegment::erase(Byte eraser) {
	if (this->mass_ == 1)
		throw false;
	--this->mass_;
	this->data_[this->mass_] = eraser;
	this->edited_ = true;
}

Bool BufferSegment::split(Nat index) {
	if (index >= this->mass_)
		throw false;
	new BufferSegment(this);
	Bool next = index + 1 >= this->mass_ / 2;
	this->next_->fill(&this->data_[index], this->mass_ - index);
	this->edited_ = true;
	return next;
}

Void BufferSegment::fill(const Byte *data, Nat count) noexcept {
	for (this->mass_ = 0; this->mass_ < count; ++this->mass_)
		this->data_[this->mass_] = data[this->mass_];
	this->edited_ = true;
}

}
