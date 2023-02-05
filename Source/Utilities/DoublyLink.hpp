#pragma once
#ifndef KEDIT_DOUBLYLINK_HPP
#define KEDIT_DOUBLYLINK_HPP

#include "../Types.hpp"

namespace Kedit {

template<class Data_T>
class DoublyLink {
public:
	class Node {
		friend class DoublyLink;

	public:
		inline Node(const Data_T& data, Node* above = nil)
			: under_(nil), data_(data), above_(above) {}
		
		inline Node(Node* under, const Data_T& data, Node* above = nil)
			: under_(under), data_(data), above_(above) {}

		inline ~Node() = default;

	public:
		inline Node* under() const noexcept { return this->under_; } 
		inline Data_T& data() const noexcept { return this->data_; } 
		inline Node* above() const noexcept { return this->above_; } 

	public:
		Void append();

	private:
		Node* under_;
		Data_T data_;
		Node* above_;
	};

public:
	inline DoublyLink() noexcept
		: root_(nil), head_(nil) {}

	~DoublyLink() noexcept;

public:
	inline Data_T& root() noexcept { return this->root_->data; }
	inline Data_T& head() noexcept { return this->head_->data; }
	inline Length length() const noexcept { return this->length_; } 

private:
	Node* root_;
	Node* head_;
	Length length_;
};

}

#endif
