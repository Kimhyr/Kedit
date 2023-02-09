#pragma once

#include <string_view>
#include <fstream>
#include <stack>
#include <iostream>

#include "../ANSI.h"

namespace Kedit {

enum class MessageType {
	ERROR = '0',
	NOTE = '2',
	WARNING = '3',
};

struct Message {
public:
	MessageType type;
	std::string_view string;

public:
	Message(MessageType type, const std::string_view& string)
		: type(type), string(string) {}
	Message(MessageType type, std::string_view&& string)
		: type(type), string(string) {}

public:
	void print(std::ostream& stream = std::cout) {
		char esc[] = {'\e', '[', '1', ';', '9', 0, 'm'};
		esc[5] = static_cast<char>(this->type);
		stream << esc;
		switch (this->type) {
		case MessageType::ERROR: stream << "ERROR: "; break;
		case MessageType::NOTE: stream << "NOTE: "; break;
		case MessageType::WARNING: stream << "WARNING: "; break;
		}
		stream << this->string << "\e[0m\n";
	}
};

// TODO: Use an IO stream interface.
// TODO: Move all the implementations in a source file.
class ChatBuffer {
public:
	ChatBuffer() = default;

public:
	const Message& last() const noexcept { return this->_messages.top(); }
	natptr size() const noexcept { return this->_messages.size(); }

public:
	inline void send(Message&& message) {
		this->_messages.push(message);
		message.print();
	}
	
	void flush(const char* path) {
		std::ofstream file(path);
		for (; !this->_messages.empty(); this->_messages.pop())
			this->_messages.top().print(file);
	}

private:
	std::stack<Message> _messages;
};

}
