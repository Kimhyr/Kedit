#include "TextBuffer.h"

namespace Kedit {

TextBuffer::TextBuffer(char const* filePath, Flag flags)
	: _flags(flags), _root(new Segment(false)), _head(this->_root), _height(0),
	  _cursor(*this) {
	ifstream file(filePath);
	Segment* curr = this->_root;
	for (;; curr = new Segment(*curr)) {
		for (;; curr = curr->next()) {
			byte byte;
			try { byte = file.get(); }
			catch (exception const&) { goto Escape; }
			try {
				curr->write(byte);
				if (byte == '\n')
					++this->_height;
			} catch (exception const&) { break; }
		}
	}
	this->_head = curr;
Escape:
	return;
}

TextBuffer::~TextBuffer() {
	this->forEach([](const Segment& seg){
		delete &seg;
	});
}

void TextBuffer::print() const noexcept {
	this->forEach([](const Segment& seg){
		seg.print();
	});
}

void TextBuffer::forEach(void (*proc)(const Segment&)) const {
	for (Segment* seg = this->_root; seg; seg = seg->next())
		proc(seg);
}

void TextBuffer::Cursor::write(const Segment::ContainerType::ViewType& view) {
	for (auto input : view) {
		if (!this->_segment->full())
		Write:
			this->_segment->write(input);
		else {
			this->_segment = new Segment(this->_segment);
			goto Write;
		}
	}
}

void TextBuffer::Cursor::write(byte input) {
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
			new Segment(this->_segment);
		else if (!this->_segment->next()->empty())
			goto CreateSegment;
		this->climb(*this->_segment->next());
	} else if (this->climbing())
		this->_segment->split(this->index());
Write:
	this->_segment->write(input);
	++this->_pointer;
	if (input == '\n') {
		--this->_position.row;
		this->_position.column = this->getColumn();
	} else --this->_position.column;
	this->_column = this->_position.column;
}

void TextBuffer::Cursor::erase() {
	if (this->holding()) {
		if (this->fall())
			goto Erase;
		throw out_of_range(__FUNCTION__);
	} else if (this->resting())
		this->_segment->shift();
	else if (this->hanging())
		goto Erase;
	else if (this->climbing())
		this->_segment->split(this->index());
Erase:
	bool nl = this->current() == '\n';
	this->_segment->erase();
	--this->_pointer;
	if (nl) {
		--this->_position.row;
		this->_position.column = this->getColumn();
	} else --this->_position.column;
	this->_column = this->_position.column;
}

length TextBuffer::Cursor::getColumn() {
	length i = 0;
	for (Segment* segment = this->_segment; segment;
	     segment = segment->prior()) {
		for (byte const* it = segment->end() - 1; it != segment->begin()- 1;
		     --it, ++i) {
			if (*it == '\n')
				break;
		}
	}
	return i + 1;
}

void TextBuffer::Cursor::climb(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = this->segment()->begin();
	this->_pointer -= segment.empty();
}

void TextBuffer::Cursor::drop(Segment& segment) noexcept {
	this->_segment = &segment;
	this->_pointer = (byte*)this->_segment->end() - 1;
}

bool TextBuffer::Cursor::jump() noexcept {
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

bool TextBuffer::Cursor::fall() noexcept {
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

TextBuffer::Segment::Segment(Segment& prior, bool edited) noexcept
	: _prior(&prior), _edited(edited), _next(nullptr) {
	if (prior._next)
		prior._next->_prior = this;
	prior._next = this;
}

void TextBuffer::Segment::prepend(Segment& prior) {
	prior._next = this;
	if (this->_prior)
		this->_prior->_next = &prior;
	prior._prior = this->_prior;
	this->_prior = &prior;
}

TextBuffer::Segment::~Segment() noexcept {
	if (this->_prior)
		this->_prior->_next = this->_next;
	if (this->_next)
		this->_next->_prior = this->_prior;
}

void TextBuffer::Segment::split(WeightType at) noexcept {
	new Segment(*this);
	this->_next->fill(*this, &this->_data[at]);
}

void TextBuffer::Segment::fill(Segment& from, byte const* iter) noexcept {
	// TODO: We fill the segment; then, when done filling, update from.mass with -= n.
	for (byte const* end = from.end(); iter != end; ++iter, from.erase())
		this->write(*iter);
}

void TextBuffer::Segment::print() const noexcept {
	for (auto bit : *this)
		cout << bit;
}

}
