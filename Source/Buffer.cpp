#include "Buffer.hpp"

#include "IO.hpp"

namespace Kedit {

///////////////////////////////////////////////////////////////////////////////
// Buffer

Buffer::~Buffer() noexcept {
	for (BufferSegment* seg; this->root_; this->root_ = seg) {
		seg = this->root_->next();
		delete this->root_;
	}
}

Void Buffer::print() {
	for (BufferSegment* segment = this->root_; segment;
	     segment = segment->next()) {
		if (segment->empty())
			continue;
		segment->print();
	}
}

Void Buffer::loadFile(const Sym* path) {
	File file(path, "r");
	if (file.empty())
		file.write("\n");
	BufferSegment* segment = this->root_;
	Byte datum = file.get(); 
	for (;; segment = new BufferSegment(*segment)) {
		try {
			for (;; datum = file.get()) {
				if (datum == '\n')
					++this->rows_;
				try { segment->write(datum); }
				catch (ErrorCode) { break; }
			}
		} catch (ErrorCode) {
			break;
		}
	}
	file.close();
}

///////////////////////////////////////////////////////////////////////////////
// BufferCursor

Void BufferCursor::write(Bit bit) noexcept {
	if (this->segment_.full() && this->hanging()) {
		if (this->segment_.next()->empty())
			this->climb(*this->segment_.next());
		else this->climb(*new BufferSegment(this->segment_));
		this->segment_.fill(*this->segment_.prior(), (Bit*)this->pointer_);
	} else if (this->beginning()) {
		if (!this->segment_.prior()) {
			puts("BREAK");
			this->drop(*new BufferSegment(this->segment_));
			this->segment_.print();
		} else {
			if (this->segment_.prior()->full())
				this->drop(*new BufferSegment(this->segment_));
			else this->drop(*this->segment_.prior());
		}
	} else {
		if (!this->segment_.next()->empty())
			new BufferSegment(this->segment_);
		this->segment_.next()->fill(*this->segment_.prior(), (Bit*)this->pointer_);
	}
	this->segment_.write(bit);
	if (bit == '\n') {
		++this->position_.row;
		this->position_.column = 0;
	} else ++this->position_.column;
	this->column_ = this->position_.column;
	++this->pointer_;
}

Void BufferCursor::erase() {
	Bool nl = this->current() == '\n';
	if (this->beginning()) {
		if (!this->segment_.prior())
			throw ErrorCode::OOR;
		this->segment_.shift();
		this->fall();
	} else if (!this->hanging()) {
		this->segment_.split((Bit*)this->pointer_);
		--this->pointer_;
	}
	this->segment_.erase();
	if (nl) {
		++this->position_.row;
		this->position_.column = 1;
	} else ++this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::fall() noexcept {
	for (BufferSegment* curr = this->segment_.prior(); curr != nil;
	     curr = curr->prior()) {
		if (curr->empty())
			continue;
		this->segment_ = *curr;
		this->pointer_ = this->segment_.end() - 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// BuffeSegment

BufferSegment::BufferSegment(BufferSegment& prior) noexcept
	: edited_(false), end_(this->data_), next_(nil) {
	prior.next_ = this;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}

BufferSegment::~BufferSegment() noexcept {
	if (this->prior_)
		this->prior_->next_ = this->next_;
	if (this->next_)
		this->next_->prior_ = this->prior_;
}

Void BufferSegment::write(Bit bit) {
	if (this->mass() == CAPACITY)
		throw ErrorCode::OVERFLOW;
	*this->end_ = bit;
	++this->end_;
}

Void BufferSegment::erase() {
	if (this->empty())
		throw ErrorCode::UNDERFLOW;
	--this->end_;
}

Void BufferSegment::shift() {
	for (Bit* it = this->data_ + 1; it != this->end_; ++it)
		it[-1] = *it;
	--this->end_;
}

Void BufferSegment::split(Bit* from) {
	new BufferSegment(*this);
	this->next_->fill(*this, from);
}

Void BufferSegment::fill(BufferSegment& from, Bit* it) noexcept {
	const Bit* end = from.end_;
	for (; it != end; ++it, ++this->end_, --from.end_) {
		*this->end_ = *it;
	}
}

Void BufferSegment::print() {
	for (const Bit* it = this->data_; it != this->end_; ++it)
		printf("%c", *it);
}

/////////////////////////////////mass/////////////////////////////////////////////
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
