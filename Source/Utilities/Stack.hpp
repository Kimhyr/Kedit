#pragma once
#ifndef KEDIT_STACK_HPP
#define KEDIT_STACK_HPP

#include "../Types.hpp"

namespace Kedit {

template<class Object_T>
class Stack {
private:
	struct Frame {
		Object_T object;
		Frame* prior;

		inline Frame(const Object_T& object, Frame* prior = nil)
			: object(object), prior(prior) {}

		inline ~Frame() {
			this->object.~Object_T();
		}
	};
	
public:
	Stack()
		: top_(nil), height_(0) {}

	~Stack();

public:
	inline Object_T& top() noexcept { return this->top_->object; };
	inline Length height() const noexcept { return this->height_; }

	constexpr Bool empty() const noexcept { return this->top_ == nil; }

public:
	Void put(const Object_T& object) noexcept;

	Void pop();

private:
	Frame* top_;
	Nat height_;
};

}

#endif
