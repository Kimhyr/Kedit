#include "Text_Buffer.h"

#include <stdexcept>

namespace Kedit {

void Text_Cursor::write(std::string_view const& input) noexcept {
	if (this->is_holding()) {
		if (this->bucket().is_empty())
			goto Write;
		if (this->is_at_root()) {
                        this->bucket_->prepend(*new Text_Bucket(true));
               		this->wheel_.root_ = this->bucket_->prior_;
		} else if (!this->bucket().prior().is_empty())
			this->bucket_->prepend(*new Text_Bucket(true));
		this->drop_to(*this->bucket_->prior_);
	} else if (this->is_climbing())
		this->bucket_->split(this->depth());
Write:
	for (Bit_Type piece : input) {
		if (this->bucket().is_full()) {
			new Text_Bucket(*this->bucket_, true);
			if (this->is_at_head())
				this->wheel_.root_ = this->bucket_->prior_;
			// We are using `drop_to` instead of `jump_to` because we already know that
			// the next bucket is empty.
			this->drop_to(*this->bucket_->prior_);
		}
		this->bucket_->put(piece);
		++this->pointer_;
		if (piece == '\n') {
			++this->position_.row;
			++this->wheel_.height_;
			this->position_.column = 1;
		} else ++this->position_.column;
		this->column_ = this->position_.column;
	}
}

void Text_Cursor::erase(Weight_Type diff) {
	if (this->is_holding())
		// We are holdin 
		throw std::out_of_range("Trying to erase an empty buffer.");
	else if (this->is_climbing())
		this->bucket_->split(this->depth());
	for (; diff; --diff) {
		if (this->bucket().is_empty()) {
			if (this->is_at_root())
				throw std::out_of_range("Trying to erase before the buffer.");
			this->drop_to(*this->bucket_->prior_);
		}
		bool is_nl = this->surface_data() == '\n';
		this->bucket_->take();
		--this->pointer_;
		if (is_nl) {
			--this->position_.row;
			--this->wheel_.height_;
			this->position_.column = this->compute_column();
		} else --this->position_.column;
		this->column_ = this->position_.column;
	}
}

void Text_Cursor::drop_to(Bucket& bucket) noexcept {
	this->pointer_ = bucket.end() - 1;
	this->bucket_ = &bucket;
}

void Text_Cursor::jump_to(Bucket& bucket) noexcept {
	this->pointer_ = bucket.begin();
	this->bucket_ = &bucket;
	// If a bucket is empty, "hold" onto the bucket.
	this->pointer_ -= bucket.is_empty();
}

N Text_Cursor::compute_column() noexcept {
	N res = 1;
	Bucket const* bucket = &this->bucket();
	for (Bit_Type const* iter = this->pointer(); *iter != '\n'; --iter, ++res) {
		if (iter == bucket->begin()) {
			if (bucket == this->wheel().root_)
				break;
			bucket = bucket->prior_;
			iter = bucket->end() - 1;
		}
	}
	return res;
}

void Text_Bucket::put(Bit_Type water) {
	if (this->is_full())
		throw std::overflow_error(__FUNCTION__);
	*this->surface_ = water;
	++this->surface_;
}

void Text_Bucket::take(Weight_Type count) {
	if (this->is_empty())
		throw std::underflow_error(__FUNCTION__);
	this->surface_ -= count;
}

void Text_Bucket::drip(Weight_Type count) {
	if (this->is_empty())
		throw std::underflow_error(__FUNCTION__);
	std::shift_left(this->data_, this->surface_, count);
	this->surface_ -= count;
}

Text_Bucket& Text_Bucket::split(Weight_Type at) {
	auto bucket = new Text_Bucket(*this, true);
	Weight_Type i = at;
	for (; i < this->weight(); ++i)
		bucket->data_[i - at] = this->data_[i];
	bucket->surface_ += i;
	return *bucket;
}

}
