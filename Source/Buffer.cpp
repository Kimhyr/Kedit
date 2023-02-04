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
				} catch (ErrorCode) { break; }
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
	// not doing `this->hanging() && this->segment_->full()`
	// because it makes a bug.
	if (this->segment_->full()) {
		std::cout << "full\n";
		if (!this->segment_->next()) {
		std::cout << "!next\n";
		CreateSegment:
			new BufferSegment(*this->segment_);
		} else if (!this->segment_->next()->empty()) {
		std::cout << "!empty\n";
			goto CreateSegment;
		}
		this->climb(*this->segment_->next());
	} else if (this->resting()) {
		std::cout << "resting\n";
		if (!this->segment_->empty()) {
			std::cout << "!empty\n";
			if (!this->segment->prior()) {
				std::cout << "!prior\n";
				this->segment_->prepend(*new BufferSegment);
			} else if (!this->segment_->prior()->empty()) {
				std::cout << "!prior.empty\n";
				new BufferSegment(*this->segment_->prior());	
			}
			this->drop(*this->segment_->prior());
		}

	}
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
YouTubeRewind:
	if (this->hanging())
		throw ErrorCode::OUT_OF_RANGE;
	if (this->segment_->empty()) {
		std::cout << "empty\n";
		if (!this->fall()) {
			if (!this->jump())
				throw ErrorCode::OUT_OF_RANGE;
		}
		goto YouTubeRewind;
	} else if (this->resting()) {
		std::cout << "resting\n";
		this->segment_->shift();
	} else if (!this->hanging()) {
		std::cout << "!hanging\n";
		this->segment_->split((Bit*)this->pointer_);
	}
	this->segment_->erase();
Epilogue:
	if (nl) {
		--this->position_.row;
		this->position_.column = this->segment_->mass() - 1;
	} else ++this->position_.column;
	this->column_ = this->position_.column;
}

Void BufferCursor::climb(BufferSegment& segment) noexcept {
	this->segment_ = &segment;
	this->pointer_ = this->segment_->start();
	// this->pointer_ -= segment.empty();
}

Void BufferCursor::drop(BufferSegment& segment) noexcept {
	this->segment_ = &segment;
	this->pointer_ = this->segment_->end() - 1;
}

Bool BufferCursor::jump() noexcept {
	std::cout << "jumping\n";
	// TODO: Figure something out to not save the orginal location (`orig`).
	BufferSegment* orig = this->segment_;
	BufferSegment* curr = this->segment_->next();
	for (; curr; curr = curr->next()) {
		if (curr->empty())
			continue;
		this->climb(*curr);
		this->segment_->print();
		std::cout << '\n';
		break;
	}
	return orig != this->segment_;
}

Bool BufferCursor::fall() noexcept {
	std::cout << "falling\n";
	// TODO: Figure something out to not save the orginal location (`orig`).
	BufferSegment* orig = this->segment_;
	BufferSegment* curr = this->segment_->prior();
	for (; curr; curr = curr->prior()) {
		if (curr->empty())
			continue;
		this->drop(*curr);
		this->segment_->print();
		std::cout << '\n';
		break;
	}
	return orig != this->segment_;
}

///////////////////////////////////////////////////////////////////////////////
// BuffeSegment

BufferSegment::BufferSegment(BufferSegment& prior) noexcept
	: edited_(false), end_(this->data_), next_(prior.next_) {
	prior.next_ = this;
	this->prior_ = &prior;
}

BufferSegment::~BufferSegment() noexcept {
	if (this->prior_)
		this->prior_->next_ = this->next_;
	if (this->next_)
		this->next_->prior_ = this->prior_;
}

Void BufferSegment::write(Bit bit) {
	if (this->full())
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
	std::cout << "shifting\n";
	for (Bit* it = this->data_ + 1; it != this->end_; ++it)
		it[-1] = *it;
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
	if (it == from.end_)
		std::cout << "Hello";
}

Void BufferSegment::prepend(BufferSegment& prior) {
	prior.next_ = this;
	prior.prior_ = this->prior_;
	this->prior_ = &prior;
}

Void BufferSegment::print() {
	for (const Bit* it = this->data_; it != this->end_; ++it)
		std::cout << *it;
}

}