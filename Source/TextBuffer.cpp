#include "TextBuffer.h"

#include <fstream>

namespace Kedit {

TextBuffer::TextBuffer(const char* filePath, Flag flags)
	: _flags(flags), _root(new Segment(false)), _weight(0), _height(0),
	  _cursor(*this) {
	std::ifstream file(filePath);
	Segment* curr{this->_root};
	for (;; curr = new Segment(*curr, false)) {
		for (;;) {
			if (!file.good() || file.eof())
				goto Escape;
			if (curr->full())
				break;
			byte byte = file.get(); // Ignore narrowing.
			curr->write(byte);
			if (byte == '\n')
				++this->_height;
		}
		this->_weight += curr->weight();
	}
Escape:
	this->_weight = curr->weight() - 1;
	curr->_next = this->_root;
	this->_root->_prior = curr;
	return;
}

TextBuffer::~TextBuffer() {
	const Segment* head{this->root()->prior()};
	for (Segment* next; this->root(); this->_root = next) {
		if (this->root() == head)
			break;
		next = this->_root->_next;
		delete this->_root;
	}
	delete this->_root;
}

void TextBuffer::print() const noexcept {
	for (const Segment* curr{this->root()}; curr; curr = curr->next()) {
		curr->print();
		if (curr == this->root()->prior())
			break;
	}
}

void TextBufferCursor::write(const View& view) {
	for (byte input : view) {
		if (this->holding()) {
			if (this->segment()->empty())
				goto Write;
			if (this->rooting()) {
				this->_segment->prepend(*new Segment(true));
			}
		}
	Write:
		this->_segment->write(input);
		++this->_buffer._weight;
		++this->_pointer;
		if (input == '\n') {
			++this->_position.row;
			++this->_buffer._height;
			// TODO: Update column.
		} else ++this->_position.column;
		this->_column = this->_position.column;
	}
}

}
