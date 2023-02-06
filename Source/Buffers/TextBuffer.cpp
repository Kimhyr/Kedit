#include "TextBuffer.h"

namespace Kedit {

TextBuffer::TextBuffer(const View<Bit>* view, Flag flags) noexcept
	: _flags(flags), _cursor(*this->_root) {
	const Bit* it = view->begin();
	for (Segment* curr = this->_root;; curr = new Segment(*curr)) {
		for (;; ++it) {
			if (curr->full())
				break;
			if (it >= view->end())
				goto Escape;
			curr->write(*it);
			if (*it == '\n')
				++this->_height;
		}
	}
Escape:
	return;
}

TextBuffer::~TextBuffer() {
	for (Segment* seg; this->_root; this->_root = seg) {
		seg = this->_root->_next;
		delete this->_root;
	}
}

Void TextBuffer::print() noexcept {
	while (this->_root->prior())
		this->_root = this->_root->prior();
	for (Segment* segment = this->_root; segment;
	     segment = segment->next()) {
		if (segment->empty())
			continue;
		segment->print();
	}
	std::cout << '\n';
}

Void TextBuffer::Cursor::write(Bit bit) {
	if (this->holding()) {
		if (this->_segment->empty())
			goto Write;
		if (!this->_segment->prior())
			this->_segment->prepend(*new Segment);
		else if (!this->_segment->prior()->empty())
			new Segment(*this->_segment->prior());
		this->drop(*this->_segment->prior());
	} else if (this->hanging()) {
		if (!this->_segment->full())
			goto Write;
		if (!this->_segment->next())
		CreateSegment:
			new Segment(*this->_segment);
		else if (!this->_segment->next()->empty())
			goto CreateSegment;
		this->climb(*this->_segment->next());
	} else if (this->climbing())
		this->_segment->split(this->_pointer);
Write:
	this->_segment->write(bit);
}

Void TextBuffer::Cursor::erase() {
	if (this->holding()) {
		if (this->fall())
			goto Erase;
		throw Error(0);
	} else if (this->resting())
		this->_segment->shift();
	else if (this->hanging())
		goto Erase;
	else if (this->climbing())
		this->_segment->split(this->_pointer);
Erase:
	Bool nl = this->current() == '\n';
	this->_segment->erase();
	--this->_pointer;
	if (nl) {
		--this->_position.row;
		this->_position.column = this->getColumn();
	} else --this->_position.column;
	this->_column = this->_position.column;
}

Nat TextBuffer::Cursor::getColumn() {
	Nat i = 0;
	for (Segment* segment = this->_segment; segment;
	     segment = segment->prior()) {
		for (const Bit* it = segment->end() - 1; it != segment->begin()- 1;
		     --it, ++i) {
			if (*it == '\n')
				break;
		}
	}
	return i + 1;
}

Void TextBuffer::Cursor::climb(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = this->_segment->begin();
	this->_pointer -= segment.empty();
}

Void TextBuffer::Cursor::drop(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = this->_segment->end() - 1;
}

Bool TextBuffer::Cursor::jump() noexcept {
	Segment* orig = this->_segment;
	Segment* curr = this->_segment->next();
	for (; curr; curr = curr->next()) {
		if (curr->empty())
			continue;
		this->climb(*curr);
		break;
	}
	return orig != this->_segment;
}

Bool TextBuffer::Cursor::fall() noexcept {
	Segment* orig = this->_segment;
	Segment* curr = this->_segment->prior();
	for (; curr; curr = curr->prior()) {
		if (curr->empty())
			continue;
		this->drop(*curr);
		break;
	}
	return orig != this->_segment;
}

TextBuffer::Segment::Segment(Segment& prior) noexcept
	: _prior(&prior) {
	if (prior._next)
		prior._next->_prior = this;
	prior._next = this;
}

Void TextBuffer::Segment::prepend(Segment& prior) {
	prior._next = this;
	if (this->_prior)
		this->_prior->_next = this->_prior;
	prior._prior = this->_prior;
	this->_prior = &prior;
}

TextBuffer::Segment::~Segment() noexcept {
	if (this->_prior)
		this->_prior->_next = this->_next;
	if (this->_next)
		this->_next->_prior = this->_prior;
}

Void TextBuffer::Segment::split(Bit* from) noexcept {
	new Segment(*this);
	this->_next->fill(*this, from);
}

Void TextBuffer::Segment::fill(Segment& from, Bit* iter) noexcept {
	// TODO: We fill the segment; then, when done filling, update from.mass with -= n.
	for (const Bit* end = from.end(); iter != end; ++iter, from.erase())
		this->write(*iter);
}


Void TextBuffer::Segment::print() noexcept {
	for (auto bit : *this)
		std::cout << bit;
}

}
