#pragma once
#ifndef KEDIT_CHATBUFFER_HPP
#define KEDIT_CHATBUFFER_HPP

#include "../Utilities/Stack.hpp"
#include "../Utilities/File.hpp"
#include "../Utilities/String.hpp"
#include "Buffer.hpp"

namespace Kedit {

// Used for debug messages.
class ChatBuffer: public Buffer<Text, Stack<Text>, File> {
public:
	ChatBuffer() = default;

	~ChatBuffer() = default;

public:
	inline const Stack<Text>& container() const noexcept override { return this->texts_; }

public:
	inline Void write(Text text) override {
		this->texts_.put(text);
	}

	Void flush(File& out) override;

private:
	Stack<Text> texts_;
};

}

#endif
