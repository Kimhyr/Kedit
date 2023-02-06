#include "text_buffer.h"

#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace kedit {

text_buffer::text_buffer(std::ifstream& file)
	: _segments(1), _cursor(this->_segments.begin()) {
	list_type::iterator curr = this->_segments.begin();
	while (file.good()) {
		curr->_edited = true;
		for (auto& bit : *curr) {
			file.get(bit);
			if (bit == '\n')
				++this->_height;
			if (!file.good())
				goto escape;
		}
		this->_segments.push_back(text_buffer_segment());
		curr = std::next(curr);
	}
escape:
	return;
}

void text_buffer_cursor::write(bit bit) {
	if (this->holding()) {
		if (this->_segment->empty())
			goto write;
		if (!this->_segment._M_node->_M_prev)
			
		else if ()
	}

write:
	this->_segment->write(bit);
	++this->_index;
	if (bit == '\n') {
		++this->_location.row;
		this->_location.column = 1;
	} else ++this->_location.column;
	this->_column = this->_location.column;
}

void text_buffer_segment::write(bit bit) {
	if (this->full())
		throw std::overflow_error("");
	this->_data[this->mass()] = bit;
}

void text_buffer_segment::write(const std::basic_string<bit>& bits) {
	if (this->mass() + bits.length() > text_buffer_segment::capacity)
		throw std::overflow_error("");
	for (size_type i = 0; i < bits.length(); ++i)
		this->_data[this->mass()] = std::move(bits[i]);
}

void text_buffer_segment::erase(size_type count) {
	if (this->_data.empty())
		throw std::underflow_error("");
	this->_mass -= count;
}

}
