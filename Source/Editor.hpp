#pragma once
#ifndef KEDIT_EDITOR_HPP
#define KEDIT_EDITOR_HPP

#include "Buffers/ChatBuffer.hpp"
#include "Buffers/TextBuffer.hpp"

namespace Kedit {

class Editor {
private:
	TextBuffer text_; // The buffer the user will be writing to before flushing into a file.
	ChatBuffer chat_; // Warnings, errors, notes, and other types of messages.
};

}

#endif // KEDIT_EDITOR_HPP
