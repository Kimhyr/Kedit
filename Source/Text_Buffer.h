#pragma once

#include <string_view>
#include <stdexcept>
#include <algorithm>

#include "Types.h"

namespace Kedit {

class Text_Buffer {
	friend class Bucket;
	friend class Cursor;

public:
	class Bucket {
		friend class Text_Buffer;
	
	public:
		using This_Type = Bucket;
		using Bit_Type = B8;
		using Weight_Type = N16;
	
		static constexpr const Weight_Type CAPACITY = 1024;
	
	public:
		constexpr Bucket(bool filled) noexcept
			: prior_(this), filled_(filled), next_(this) {} 
		constexpr Bucket(This_Type& prior, bool filled)
			: prior_(&prior), filled_(filled), next_(prior.next_) {
			prior.next_->prior_ = this;
			prior.next_ = this;
		}
		Bucket(const This_Type&) = delete;
		Bucket(This_Type&&) = delete;
	
		This_Type& operator=(This_Type const&) = delete;
		This_Type& operator=(This_Type&&) = delete;
	
		~Bucket() = default;
	
	public:
		constexpr This_Type const& prior() const noexcept { return *this->prior_; }
		constexpr This_Type const& next() const noexcept { return *this->next_; }
	
		constexpr B filled() const noexcept { return this->filled_; }
	
		constexpr const Bit_Type* data() const noexcept { return this->data_; }
	
		constexpr const Bit_Type* begin() const noexcept { return this->data_; }
		constexpr const Bit_Type* end() const noexcept { return this->surface_; }
	
		constexpr Weight_Type weight() const noexcept { return this->begin() - this->end(); }
		constexpr bool is_full() const noexcept { return this->end() == this->begin() + CAPACITY; }
		constexpr bool is_empty() const noexcept { return this->end() == this->begin(); }
	
		constexpr Bit_Type& operator[](I index) noexcept { return this->data_[index]; }
		constexpr Bit_Type& at(Weight_Type index) {
			if (index >= this->weight())
				throw std::out_of_range(__FUNCTION__);
			return this->data_[index];
		}
	
	public:
		void put(Bit_Type water);
		
		void take(Weight_Type count = 1);
		void drip(Weight_Type count = 1);
	
		Bucket& split(Weight_Type at);
	
		constexpr void prepend(This_Type& prior) noexcept {
			prior.prior_ = this->prior_;
			prior.next_ = this;
			this->prior_->next_ = &prior;
			this->prior_ = &prior;
		}
	
	private:
		This_Type* prior_;
		B filled_;
		Bit_Type data_[CAPACITY];
		Bit_Type* surface_;
		This_Type* next_;
	};

	class Cursor {
		friend class Text_Buffer;

	public:
		using This_Type = Cursor;
		using Bit_Type = Bucket::Bit_Type;
		using Weight_Type = Bucket::Weight_Type;

	public:
		constexpr Cursor(Text_Buffer& wheel) noexcept
			: wheel_(wheel), offset_(0), bucket_(wheel.root_), pointer_(wheel.root_->begin() - 1),
			  column_(0) {}

		~Cursor() = default;

	public:
		constexpr const Text_Buffer& wheel() const noexcept { return this->wheel_; }

		constexpr Bucket const& bucket() const noexcept { return *this->bucket_; }
		constexpr Bit_Type const* pointer() const noexcept { return this->pointer_; }

		constexpr Position const& position() const noexcept { return this->position_; }
		constexpr Weight_Type column() const noexcept { return this->column_; }

		constexpr Weight_Type offset() const noexcept { return this->offset_;}
		constexpr Weight_Type depth() const noexcept { return this->pointer() - this->bucket().begin(); }

		constexpr Bit_Type surface_data() const noexcept { return this->bucket().end()[-1]; }

		constexpr B is_holding() const noexcept { return this->pointer() + 1 == this->bucket().begin(); }
		constexpr B is_resting() const noexcept { return this->pointer() == this->bucket().begin(); }
		constexpr B is_hanging() const noexcept { return this->pointer() + 1 == this->bucket().end(); }
		constexpr B is_climbing() const noexcept { return this->pointer() >= this->bucket().begin() && this->pointer() < this->bucket().end(); }

		constexpr B is_at_root() const noexcept { return &this->bucket() == &this->wheel().root(); }
		constexpr B is_at_head() const noexcept { return &this->bucket() == &this->wheel().root().prior(); }

	public:
		void write(std::string_view const& string) noexcept;

		void erase(Weight_Type diff);

		void move_up(Weight_Type steps);
		void move_down(Weight_Type steps);
		void move_right(Weight_Type steps);
		void move_left(Weight_Type steps);
 
		Bucket* locate_backwards(Bit_Type delimiter);
		Bucket* locate_forwards(Bit_Type delimeter);
	
	private:
		Text_Buffer& wheel_;
		Weight_Type offset_;
		Bucket* bucket_;
		Bit_Type const* pointer_; 
		Position position_;
		N column_;
	
	private:
		void drop_to(Bucket& bucket) noexcept;
		void jump_to(Bucket& bucket) noexcept;
		N compute_column() noexcept;
	};

	enum class Flag {
	
	};

public:
	constexpr Bucket const& root() const noexcept { return *this->root_; }

private:
	Bucket* root_;
	N height_;
};

using Text_Bucket = Text_Buffer::Bucket;
using Text_Cursor = Text_Buffer::Cursor;
using Text_Buffer_Flag = Text_Buffer::Flag;

}
