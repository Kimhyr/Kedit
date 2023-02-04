#include "Buffer.hpp"

#include "File.hpp"
#include "Global.hpp"

namespace Kedit {

///////////////////////////////////////////////////////////////////////////////
// Buffer

Buffer::Buffer(const Sym* filePath)
	: root_(new BufferSegment), cursor_(*this->root_), rows_(1) {
	this->loadFile(filePath);
}

Buffer::~Buffer() noexcept {
	for (BufferSegment* seg; this->root_; this->root_ = seg) {
		seg = this->root_->next();
		delete this->root_;
	}
}

Void Buffer::print() {
	while (this->root_->prior())
		this->root_ = this->root_->prior();
	for (BufferSegment* segment = this->root_; segment;
	     segment = segment->next()) {
		if (segment->empty())
			continue;
		segment->print();
	}
	std::cout << '\n';
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
				try {
					segment->write(datum);
				} catch (Error) { break; }
			}
		} catch (Error) {
			break;
		}
	}
	file.close();
}

///////////////////////////////////////////////////////////////////////////////
// BufferCursor

Void BufferCursor::write(Bit bit) {
	if (this->holding()) {
		if (!this->segment_->empty())
			goto Drop;
		if (!this->fall()) {
			if (!this->jump())
				goto Write;
			else goto Resting;
		} else goto Hanging;
	} else if (this->resting()) {
	Resting:
		if (this->hanging())
  		 	goto Write;
	Drop:
		if (!this->segment_->prior())
			this->segment_->prepend(*new BufferSegment);
		else if (!this->segment_->prior()->empty())
			new BufferSegment(*this->segment_->prior());
		this->drop(*this->segment_->prior());
	} else if (this->hanging()) {
	Hanging:
		if (!this->segment_->full())
			goto Write;
		if (!this->segment_->next())
		CreateSegment:
			new BufferSegment(*this->segment_);
		else if (!this->segment_->next()->empty())
			goto CreateSegment;
		this->climb(*this->segment_->next());
	} else if (this->climbing()) {
		this->segment_->split((Bit*)this->pointer_);
		--this->pointer_;
	} else 	throw Error(Error::UNKNOWN_DECISION);
Write:
	this->segment_->write(bit);
	++this->pointer_;
	if (bit == '\n') {
		++this->position_.row;
		this->position_.column = 0;
	} else ++this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::erase() {
	Bool nl = this->current() == '\n';
	if (this->holding()) {
		if (this->fall())
			goto Erase;
		else if (!this->segment_->empty()) {
			++pointer_;
			goto Erase;
		} else if (this->jump()) {
			this->segment_->shift();
			goto Finalize;
		} else throw Error(Error::OUT_OF_RANGE);
	}
	if (this->resting()) {
		this->segment_->shift();
		if (this->segment_->empty()) {
			if (!this->fall()) {
				if (!this->jump())
					--this->pointer_;
			}
		} else if (this->segment_->mass() == 1)
			goto DecPtr;
		goto Finalize;
	} else if (!this->hanging())
		this->segment_->split((Bit*)this->pointer_ + 1);
	else throw Error(Error::UNKNOWN_DECISION);
Erase:
	this->segment_->erase();
DecPtr:
	--this->pointer_;
Finalize:
	if (nl) {
		--this->position_.row;
		// this->position_.column = this->getColumn();
	} else ++this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::climb(BufferSegment& segment) noexcept {
	this->segment_ = &segment;
	this->pointer_ = this->segment_->start();
	this->pointer_ -= segment.empty();
}

Void BufferCursor::drop(BufferSegment& segment) noexcept {
	this->segment_ = &segment;
	this->pointer_ = this->segment_->end() - 1;
}

Bool BufferCursor::jump() noexcept {
	// TODO: Figure something out to not save the orginal location (`orig`).
	BufferSegment* orig = this->segment_;
	BufferSegment* curr = this->segment_->next();
	for (; curr; curr = curr->next()) {
		if (curr->empty())
			continue;
		this->climb(*curr);
		break;
	}
	return orig != this->segment_;
}

Bool BufferCursor::fall() noexcept {
	// TODO: Figure something out to not save the orginal location (`orig`).
	BufferSegment* orig = this->segment_;
	BufferSegment* curr = this->segment_->prior();
	for (; curr; curr = curr->prior()) {
		if (curr->empty())
			continue;
		this->drop(*curr);
		break;
	}
	return orig != this->segment_;
}

///////////////////////////////////////////////////////////////////////////////
// BuffeSegment

BufferSegment::BufferSegment(BufferSegment& prior) noexcept
	: prior_(&prior), edited_(false), end_(this->data_), next_(prior.next_) {
	if (prior.next_)
		prior.next_->prior_ = this;
	prior.next_ = this;
}

BufferSegment::~BufferSegment() noexcept {
	if (this->prior_)
		this->prior_->next_ = this->next_;
	if (this->next_)
		this->next_->prior_ = this->prior_;
}

Void BufferSegment::write(Bit bit) {
	if (this->full())
		throw Error(Error::OVERFLOW);
	*this->end_ = bit;
	++this->end_;
}

Void BufferSegment::erase() {
	if (this->empty())
		throw Error(Error::UNDERFLOW);
	--this->end_;
}

Void BufferSegment::shift() {
	std::cout << "shifting\n";
	for (Bit* it = this->data_ + 1; it != this->end_; ++it)
		it[-1] = *it;
	--this->end_;
}

Void BufferSegment::split(Bit* from) {
	std::cout << "splitting\n";
	new BufferSegment(*this);
	this->next_->fill(*this, from);
}

Void BufferSegment::fill(BufferSegment& from, Bit* it) noexcept {
	const Bit* end = from.end_;
	for (; it != end; ++it, ++this->end_, --from.end_)
		*this->end_ = *it;
}

Void BufferSegment::prepend(BufferSegment& prior) {
	prior.next_ = this;
	if (this->prior_)
		this->prior_->next_ = prior_;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}

Void BufferSegment::print() {
	for (const Bit* it = this->data_; it != this->end_; ++it)
		std::cout << *it;
}

}