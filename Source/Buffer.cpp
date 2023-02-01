#include "Buffer.hpp"

namespace Kedit {

Bool BufferCursor::atLineEnd() const noexcept {
	if (this->atSegmentEnd()) {
		BufferSegment &seg = this->segment_;
		for (; seg.next(); seg = *seg.next()) {
			if (seg.next()->mass())
				return (*seg.next())[0] == '\n';
		}
		return true;
	} else return this->segment_[this->index_ + 1] == '\n'; 
}

Void BufferCursor::write(Byte datum) noexcept {
	if (this->segment_.full()) {
		this->segment_ = (this->segment_.next() && !this->segment_.next()->mass())
			? *this->segment_.next()
			: *new BufferSegment(this->segment_);
		this->index_ = 0;
	} else if (this->index_ == 0 && this->segment_.mass()) {
		this->segment_.prepend(*new BufferSegment());
		this->segment_ = *this->segment_.prior();
	} else if (!this->atSegmentEnd()) {
		this->segment_.split(this->index_);
	}
	this->segment_.write(datum);
	++this->index_;
	if (datum == '\n') {
		++this->position_.row;
		this->position_.column = 0;
	}
	this->column_ = ++this->position_.column;
}

Void BufferCursor::erase(Byte eraser) {
	Bool nl = this->onNewLine();
	if (this->index_ == 0) {
		if (!this->segment_.prior())
			throw false;
		this->segment_.shift();
		BufferSegment &original = this->segment_;
		for (this->segment_ = *this->segment_.prior(); !this->segment_.mass();
		     this->segment_ = *this->segment_.prior()) {
			if (this->segment_.prior())
				continue;
			this->segment_ = original;
			this->index_ = 0;
			goto Update;
		}
		this->index_ = this->segment_.mass() - 1;
		goto Update;
	} else if (!this->atSegmentEnd())
		this->segment_.split(this->index_);
	this->segment_.erase(eraser);
	--this->index_;
Update:
	if (nl) {
		--this->position_.row;
		this->position_.column = this->getColumn();
	} else --this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::moveUp() {
	if (this->position_.row == 1)
		throw false;
	if (this->position_.column > 1)
		this->moveToLineStart();
	this->moveLeft();
	this->moveToLineStart();
	this->setColumns();
	// A '\n' may occur before `column_` just brak. i dont give a shit nm. I
	// havent slept yet bro I spelpt at ~5pm+ 30 Jan 2023; slept for like 4 hours
	// and stayed up. I'm in school (lunch) at 31 Jan 2023 12:29pm westwood
	// highschool, south carolina.
	// i've never been in an actual computer science class. only one with like
	// html and fuckign karel???? KAREL dumabss language. its just js but with
	// stupid "moveRight" functions. teacher even corrected me because i didnt
	// use semicolons???? and i had "abstracted" too much or something (ii split
	// up a function into like 5). I didnt even get a single class i chose this
	// school year. french -> spanish, comp sci -> biology/mwh or sumshit. And im
	// stuck in a slow ass intermediate algebra class. I DO CALCULUS IN MY FRE TIM,
	// its my fault that im in interm. algebra tho 'cause i didnt do my work nor
	// cared. I have a 59 and 50 for first and second quarter of english 2
	// respectively, BUT I GOT a 96 on the end of course exam. i hate shakesperae
}

Void BufferCursor::moveDown(Nat bufferHeight) {
	if (this->position_.row == bufferHeight)
		throw false;
	if (!this->atLineEnd())
		this->moveToLineEnd();
	this->moveRight();
	this->setColumns();
}

Void BufferCursor::moveRight() {
	if (this->atSegmentEnd()) {
		if (!this->segment_.next())
			throw false;
		this->segment_ = *this->segment_.next();
		this->index_ = 0;
	} else ++this->index_;
	if (this->onNewLine()) {
		++this->position_.row;
		this->position_.column = 0;
	}
	this->column_ = ++this->position_.column;
}

Void BufferCursor::moveLeft() {
	Bool nl = this->onNewLine();
	if (this->index_ == 0) {
		if (!this->segment_.prior())
			throw false;
		this->segment_ = *this->segment_.prior();
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
	Nat column = 1;
	for (Nat index = this->index_; segment[index] != '\n'; ++column) {
		if (!index) {
			if (!segment.prior())
				break;
			segment = *segment.prior();
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
	new BufferSegment(*this);
	Nat count = this->mass_ - (index + 1);
	this->next_->fill(&this->data_[index], count);
	this->mass_ -= count;
	this->edited_ = true;
}

Void BufferSegment::prepend(BufferSegment &behind) {
	this->prior_->next_ = &behind;
	behind.prior_ = this->prior_;
	this->prior_ = &behind;
	this->prior_->next_ = this;
}

BufferSegment *BufferSegment::getNextFilled() {
	BufferSegment *seg = this->next();
	for (; seg; seg = seg->next()) {
		if (seg->mass())
			return seg;
	}
	return seg;
}

Void BufferSegment::fill(const Byte *data, Nat count) noexcept {
	for (this->mass_ = 0; this->mass_ < count; ++this->mass_)
		this->data_[this->mass_] = data[this->mass_];
	this->edited_ = true;
}

Void BufferCursor::moveToLineStart() noexcept {
	do {
		if (this->index_)
			goto Continue;
		if (!this->segment_.prior())
			break;
		this->segment_ = *this->segment_.prior();
		this->index_ = this->segment_.mass();
	Continue:
		--this->index_;
		--this->position_.column;
	} while (this->position_.column != 1);
}

Void BufferCursor::moveToLineEnd() noexcept {
	do {
		if (this->atSegmentEnd()) {
			BufferSegment *seg = this->segment_.getNextFilled();
			if (seg)
				this->segment_ = *seg;
			
		}
	} while (this->segment_[this->index_] != '\n');
	this->moveLeft();
}

Void BufferCursor::setColumns() noexcept {
	for (; this->position_.column != this->column_;
	     ++this->position_.column, ++this->index_) {
		if (this->segment_[this->index_] == '\n')
			break;
		if (this->index_ != this->segment_.mass())
			continue;
		this->segment_ = *this->segment_.next();
		this->index_ = 0;
	}
}

}
