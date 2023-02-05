#pragma once
#ifndef KEDIT_TEXTBUFFER_HPP
#define KEDIT_TEXTBUFFER_HPP

#include "../Utilities/File.hpp"
#include "Buffer.hpp"

namespace Kedit {

class TextBuffer: public Buffer<Bit, Int, File> {
};

}

#endif // KEDIT_TEXTBUFFER_HPP
