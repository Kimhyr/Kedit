#include "ChatBuffer.hpp"

namespace Kedit {

Void ChatBuffer::flush(File& out) {
	for (; !this->texts_.empty(); this->texts_.pop())
		out.write(this->texts_.top().data());
}

}
