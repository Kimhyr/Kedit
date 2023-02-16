#include "WaterWheel.h"

namespace Kedit {

void WaterWheelCursor::write(std::string_view const& input) noexcept {
	for (byte piece : input) {
		// TODO: Once the cursor is ready to simply "put" into to the bucket, change
		// the instruction course to only base on appending a bucket.
		if (this->is_holding()) {
			if (this->bucket().is_empty())
				goto Write;
			if (this->is_at_root()) {
                                this->bucket_->prepend(*new WaterBucket(true));
                		this->wheel_.root_ = this->bucket_->prior_;
			} else if (!this->bucket().prior().is_empty())
				this->bucket_->prepend(*new WaterBucket(true));
			this->drop_to(*this->bucket_->prior_);
		} else if (this->is_hanging()) {
			if (this->bucket().is_full()) {
				new WaterBucket(*this->bucket_, true);
				if (this->is_at_head())
					this->wheel_.root_ = this->bucket_->prior_;
				this->drop_to(*this->bucket_->prior_);
			}
		} else this->bucket_->split(this->depth());
        Write:
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

void WaterWheelCursor::erase(DifferenceType diff) {
	for (; diff; --diff) {
		// TODO: Likewise to handling the "write" operation, after the cursor is
		// ready to simply "take" from the bucket, change the instruction course to
		// only be based on erasing a bucket.
		if (this->is_holding()) {
			
		}
	}
}

void WaterWheelCursor::drop_to(Bucket& bucket) noexcept {
	this->pointer_ = bucket.end() - 1;
	this->bucket_ = &bucket;
}

void WaterWheelCursor::jump_to(Bucket& bucket) noexcept {
	this->pointer_ = bucket.begin();
	this->bucket_ = &bucket;
	// If a bucket is empty, "hold" onto the bucket.
	this->pointer_ -= bucket.is_empty();
}

void WaterBucket::put(byte water) {
	if (this->is_full())
		throw std::overflow_error(__FUNCTION__);
	*this->surface_ = water;
	++this->surface_;
}

void WaterBucket::take(WeightType count) {
	if (this->is_empty())
		throw std::underflow_error(__FUNCTION__);
	this->surface_ -= count;
}

void WaterBucket::drip(WeightType count) {
	if (this->is_empty())
		throw std::underflow_error(__FUNCTION__);
	std::shift_left(this->water_, this->surface_, count);
	this->surface_ -= count;
}

WaterBucket& WaterBucket::split(WeightType at) {
	auto bucket = new WaterBucket(*this, true);
	WeightType i = at;
	for (; i < this->weight(); ++i)
		bucket->water_[i - at] = this->water_[i];
	bucket->surface_ += i;
	return *bucket;
}

}
