#include "TextBuffer.h"

namespace Kedit {

TextBuffer::TextBuffer(const char* filePath, Flag flags)
	: _flags(flags), _root(new Segment(false)), _height(0), _cursor(*this), _weight(0) {
	std::ifstream file(filePath);
	Segment *curr = this->_root;
	for (;; curr = new Segment(*curr, false)) {
		for (;;) {
			if (!file.good())
				goto Escape;
			if (curr->full())
				break;
			byte byte = file.get();
			curr->write(byte);
			if (byte == '\n')
				++this->_height;
		}
		this->_weight += curr->weight();
	}
Escape:
	this->_weight = curr->weight() - 1;
	this->_head = curr;
	return;
}

TextBuffer::~TextBuffer() {
	for (Segment* next; this->_root; this->_root = next) {
		next = this->_root->_next;
		delete this->_root;
	}
}

void TextBuffer::print() const noexcept {
	for (Segment* curr = this->_root; curr; curr = curr->_next)
		curr->print();
}

void TextBufferCursor::write(const ViewType& view) {
	for (byte input : view) {
		if (this->holding()) {
			if (this->_segment->empty())
				goto Write;
			if (!this->_segment->prior()) {
				this->_segment->prepend(*new Segment(true));
				this->_buffer._root = this->_segment->_prior;
			} else if (!this->_segment->prior()->empty())
				new Segment(*this->_segment->prior(), true);
			this->drop(*this->_segment->prior());
		} else if (this->hanging()) {
			if (!this->_segment->full())
				goto Write;
			if (!this->_segment->next()) {
				new Segment(*this->_segment, true);
				this->_buffer._head = this->_segment->_next;
			} else if (!this->_segment->next()->empty())
				new Segment(*this->_segment, true);
			this->climb(*this->_segment->next());
		} else if (this->climbing())
			this->_segment->split(this->index() + 1);
	Write:
		this->_segment->write(input);
		++this->_buffer._weight;
		++this->_pointer;
		if (input == '\n') {
			--this->_position.row;
			// TODO: Make a more performant `getColumn` method.
			// this->_position.column = this->getColumn();
		} else --this->_position.column;
		this->_column = this->_position.column;
	}
}

void TextBufferCursor::erase(natptr count) {
	for (; count; --count) {
		if (this->holding()) {
			if (this->fall())
				goto Erase;
			throw std::out_of_range(__FUNCTION__);
		} else if (this->resting())
			this->_segment->shift();
		else if (this->hanging())
			goto Erase;
		else if (this->climbing()) {
			this->_segment->split(this->index() + 1);
		}
	Erase:
		bool nl = this->current() == '\n';
		this->_segment->erase();
		--this->_buffer._weight;
		--this->_pointer;
		if (nl) {
			--this->_position.row;
			// this->_position.column = this->getColumn();
		} else --this->_position.column;
		this->_column = this->_position.column;
	}
}

void TextBufferCursor::moveRight(natptr count) {
	for (; count; --count) {
		if (this->hanging()) {
			if (!this->jump())
				throw std::out_of_range(__FUNCTION__);
			goto End;
		}
		++this->_pointer;
	End:
		continue;
	}
}

void TextBufferCursor::moveLeft(natptr count) {
	for (; count; --count) {
		if (this->holding()) {
			if (!this->fall())
				throw std::out_of_range(__FUNCTION__);
			goto End;
		}
		--this->_pointer;
	End:
		continue;
	}
}

void TextBufferCursor::moveUp(natptr) {
	for (; count; --count) {
		State state = this->locateForwards('\n');
		// set our state to the found state.
		continue;
	}
}

void TextBufferCursor::moveDown(natptr count) {
	for (; count; --count) {
		State state = this->locateForwards('\n');
		// set our state to the found state.
		continue;
	}
}

natptr TextBufferCursor::getColumn() {
	natptr i = 0;
	for (Segment* segment = this->_segment; segment;
	     segment = segment->prior()) {
		for (const BitType* it = segment->end() - 1; it != segment->begin()- 1;
		     --it, ++i) {
			if (*it == '\n')
				break;
		}
	}
	return i + 1;
}

void TextBufferCursor::climb(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = this->segment()->begin();
	this->_pointer -= segment.empty();
}

void TextBufferCursor::drop(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = (BitType*)this->_segment->end() - 1;
}

bool TextBufferCursor::jump() noexcept {
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

bool TextBufferCursor::fall() noexcept {
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

TextBufferSegment::Segment(Segment& prior, bool edited) noexcept
	: _prior(&prior), _edited(edited), _next(prior._next) {
	if (prior._next)
		prior._next->_prior = this;
	prior._next = this;
}

void TextBufferSegment::prepend(Segment& prior) {
	prior._next = this;
	if (this->_prior)
		this->_prior->_next = &prior;
	prior._prior = this->_prior;
	this->_prior = &prior;
}

TextBufferSegment::~Segment() noexcept {
	if (this->_prior)
		this->_prior->_next = this->_next;
	if (this->_next)
		this->_next->_prior = this->_prior;
}

void TextBufferSegment::split(WeightType at) noexcept {
	new Segment(*this, true);
	this->_next->fill(*this, &this->_data[at]);
}

void TextBufferSegment::fill(Segment& from, const BitType* iter) noexcept {
	// TODO: We fill the segment; then, when done filling, update from.mass with -= n.
	for (const BitType* end = from.end(); iter != end; ++iter, from.erase())
		this->write(*iter);
}

void TextBufferSegment::print() const noexcept {
	for (auto bit : *this)
		std::cout << bit;
}

}
