#include "Buffer.hpp"

namespace Kedit {

Void BufferCursor::write(Byte datum) noexcept {
	// If the segment is full.
	if (this->segment_.full()) {
		// If the next segment is empty, use that empty segment; otherwise create a
		// new empty segment forwards.
		// We like empty segments, and segmented where our cursor's right side is
		// clear because we don't need to append/split/shift anything.
		this->segment_ = (this->segment_.next() && !this->segment_.next()->mass())
			? *this->segment_.next()
			: *new BufferSegment(this->segment_);
		this->index_ = 0;

	// We check for mass so that, if it does not have mass, we can just do a
	// write operation without prepending a new segment.
	} else if (this->index_ == 0 && this->segment_.mass()) {
		// Just writing at the start of the segment will require a shift, or split of
		// the segment, so prepending a new segment will allow us to just write
		// without shifting or splitting, and later writing operations will be faster
		// becuase the segment is empty.
		this->segment_.prepend(*new BufferSegment());
		this->segment_ = *this->segment_.prior();

	// If we are within the segment.
	} else if (!this->atEndOfSegment()) {
		// Reallocation is a must when writing within a segment, but, if we
		// reallocate the data from the index to a new segment, later writing in the
		// segment will not require a reallocation, nor creating a new segment, just
		// a simple write operation.
		this->segment_.split(this->index_);
	}

	// Since we are now setup to do a simple write operation to a segment, we can
	// just write into the segment.
	this->segment_.write(datum);
	++this->index_;
	
	// The buffer already knows if it should increase it's size because it was the
	// one who gave the cursor the command with '\n'!
	if (datum == '\n') {
		++this->position_.row;
		this->position_.column = 0;
	}
	// We want to save the change of left-right movement for up-down movement.
	this->column_ = ++this->position_.column;
}

Void BufferCursor::erase(Byte eraser) {
	// Checking if we passed a '\n' after erasing is inefficient because we'd need
	// a searcher to go back  and determine whether we passed a '\n' or not, so
	// let's just determine it now that we may be on it.
	Bool nl = this->onNewLine();
	if (this->index_ == 0) {
		// Since we are at the start of a segment, we may be at the begining of the
		// buffer. If we are at the beginning of the buffer, that means there is no
		// datum to erase. You can test this with your current text editor by going
		// to the start of the buffer and try erasing.
		//
		// Note that having an index of 0 and no prior segment means we are at the
		// start of the buffer.
		if (!this->segment_.prior())
			throw false;

		// Splitting the segment requires n - 1 data to be moved to a created
		// segment, but we are just erasing a measily datum. So, it is better just to
		// shift it to the left which doesn't take creating a new segment.
		this->segment_.shift();

		// We now want to move our cursor's location one to the left, but there may
		// be unused space, and a cursor must be pointed at some element. To handle
		// this, we just find the last segment prior to our segment with mass. However,
		// we might run into the start of the (empty) buffer! In this case, we go
		// back to the original segment and set our cursor to the start of that segment.
		BufferSegment &original = this->segment_;
		for (this->segment_ = *this->segment_.prior(); !this->segment_.mass();
		     this->segment_ = *this->segment_.prior()) {
			if (this->segment_.prior())
				continue;
			// The searcher hit the end of segment, so we go back to the original
			// segment.
			this->segment_ = original;
			this->index_ = 0;
			// We go to "Update" so that we don't reset our index and segment.
			goto Update;
		}
		this->index_ = this->segment_.mass() - 1;
		goto Update;
	} else if (!this->atEndOfSegment())
		// Similar to writing within a segment, it is more performant to split the
		// segment for later operations.
		this->segment_.split(this->index_);
	// We can now simply erase.
	this->segment_.erase(eraser);
	--this->index_;
Update:
	// We erased a new line, so this will handle it.
	// The buffer will handle this when it compares the differences in the
	// cursor's row position. Comparing the data[i - 1] with '\n' won't work well
	// because the buffer is not truely contigious due to it being segmented. 
	if (nl) {
		++this->position_.row;
		this->position_.column = this->getColumn();
	} else --this->position_.column;
}

// TODO: Make this more performant.
Void BufferCursor::moveUp() {
	// We can't go up a row if there are no more rows above.
	if (this->position_.row == 1)
		throw false;
	// If the cusor is not already at the start of the line, move to it.
	if (this->position_.column > 1)
		this->moveToLineStart();
	// Move to the above line, and go to the start of it.
	this->moveLeft();
	this->moveToLineStart();
	// Move up to `column_`.
	for (; this->position_.column != this->column_;
	     ++this->position_.column, ++this->index_) {
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
		if (this->segment_[this->index_] == '\n')
			break;
		if (this->index_ != this->segment_.mass())
			continue;
		this->segment_ = *this->segment_.next();
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

// We can just get the current column by counting up to the left nearest '\n'.
Nat BufferCursor::getColumn() const noexcept {
	BufferSegment &segment = this->segment_;
	Nat column = 1;
	for (Nat index = this->index_; segment[index] != '\n'; ++column) {
		if (!index) {
			// If we are at the start of the buffer, that means we can't go further.
			if (!segment.prior())
				break;
			// Moving back a segment.
			segment = *segment.prior();
			index = segment.mass() - 1;
		} else --index;
	}
	return column;
}

Void BufferSegment::write(Byte datum) {
	// Prevent overflow.
	if (this->mass_ == CAPACITY)
		throw false;
	this->data_[this->mass_] = datum;
	++this->mass_;
	this->edited_ = true;
}

Void BufferSegment::erase(Byte eraser) {
	// Prevent underflow.
	if (this->mass_ == 0)
		throw false;
	--this->mass_;
	this->data_[this->mass_] = eraser;
	this->edited_ = true;
}

Void BufferSegment::split(Nat index) {
	// Index is out of range.
	if (index >= this->mass_)
		throw false;
	new BufferSegment(*this);
	// Calculate the amount of data being removed; then, move `count` data to the
	// new segment.
	Nat count = this->mass_ - (index + 1);
	this->next_->fill(&this->data_[index], count);
	this->mass_ -= count; // Apply the loss.
	this->edited_ = true;
}

Void BufferSegment::prepend(BufferSegment &behind) {
	this->prior_->next_ = &behind;
	behind.prior_ = this->prior_;
	this->prior_ = &behind;
	this->prior_->next_ = this;
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
		// The start of the buffer is the start of a line, so break.
		if (!this->segment_.prior())
			break;
		this->segment_ = *this->segment_.prior();
		this->index_ = this->segment_.mass(); // We don't `- 1` because it'll
						      // decrement next.
	Continue:
		--this->index_;
		--this->position_.column;
	} while (this->position_.column != 1);
}

}
