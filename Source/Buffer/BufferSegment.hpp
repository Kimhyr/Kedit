#pragma once

#include "../Definitions.hpp"

namespace Kedit {

class BufferSegment {
public:
	static constexpr const Nat CAPACITY = 80;

public:
	inline BufferSegment()
		: edited_(false), mass_(0), prior_(nil), next_(nil) {}

	inline BufferSegment(BufferSegment *prior)
		: edited_(false), prior_(prior) {
		this->next_ = prior->next_;
		prior->next_ = this;
	}

	inline ~BufferSegment() {
		this->prior_->next_ = this->next_;
		this->next_->prior_ = this->prior_;
	}
public:
	inline Bool edited() const noexcept { return this->edited_; }
	inline const Byte *data() const noexcept { return this->data_; }
	inline Nat mass() const noexcept { return this->mass_; }
	inline BufferSegment *prior() noexcept { return this->prior_; }
	inline BufferSegment *next() noexcept { return this->next_; }

	inline Byte &operator [](Nat index) noexcept { return this->data_[index]; }

public:
	// Throws if `mass_ == CAPACITY`.
	Void write(Byte datum);

	// Throws if `mass_ == 1`.
	Void erase(Byte eraser = ' ');

	// Throws if `index >= mass_`.
	// Returns whether to use `next_` or not.
	Bool split(Nat index);

protected:
	Void fill(const Byte *data, Nat count) noexcept;

private:
	Bool edited_;
	Byte data_[CAPACITY];
	Nat mass_;
	BufferSegment *prior_;
	BufferSegment *next_;
};

}
