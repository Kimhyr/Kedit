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
		using Weight_Type = nat16;
		using Difference_Type = Weight_Type;
	
		
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
	
		constexpr bool filled() const noexcept { return this->filled_; }
	
		constexpr const byte* data() const noexcept { return this->data_; }
	
		constexpr const byte* begin() const noexcept { return this->data_; }
		constexpr const byte* end() const noexcept { return this->surface_; }
	
		constexpr Weight_Type weight() const noexcept { return this->begin() - this->end(); }
		constexpr bool is_full() const noexcept { return this->end() == this->begin() + CAPACITY; }
		constexpr bool is_empty() const noexcept { return this->end() == this->begin(); }
	
		constexpr byte& operator[](intptr index) noexcept { return this->data_[index]; }
		constexpr byte& at(Weight_Type index) {
			if (index >= this->weight())
				throw std::out_of_range(__FUNCTION__);
			return this->data_[index];
		}
	
	public:
		void put(byte water);
		
		void take(Difference_Type count = 1);
		void drip(Difference_Type count = 1);
	
		Bucket& split(Difference_Type at);
	
		constexpr void prepend(This_Type& prior) noexcept {
			prior.prior_ = this->prior_;
			prior.next_ = this;
			this->prior_->next_ = &prior;
			this->prior_ = &prior;
		}
	
	private:
		This_Type* prior_;
		bool filled_;
		byte data_[CAPACITY];
		byte* surface_;
		This_Type* next_;
	};

	class Cursor {
		friend class Text_Buffer;

	public:
		using This_Type = Cursor;
		using Difference_Type = natptr;

	public:
		constexpr Cursor(Text_Buffer& wheel) noexcept
			: wheel_(wheel), offset_(0), bucket_(wheel.root_), pointer_(wheel.root_->begin() - 1),
			  column_(0) {}

		~Cursor() = default;

	public:
		constexpr const Text_Buffer& wheel() const noexcept { return this->wheel_; }

		constexpr Bucket const& bucket() const noexcept { return *this->bucket_; }
		constexpr byte const* pointer() const noexcept { return this->pointer_; }

		constexpr Position const& position() const noexcept { return this->position_; }
		constexpr Difference_Type column() const noexcept { return this->column_; }

		constexpr Difference_Type offset() const noexcept { return this->offset_;}
		constexpr Difference_Type depth() const noexcept { return this->pointer() - this->bucket().begin(); }

		constexpr byte surface_data() const noexcept { return this->bucket().end()[-1]; }

		constexpr bool is_holding() const noexcept { return this->pointer() + 1 == this->bucket().begin(); }
		constexpr bool is_resting() const noexcept { return this->pointer() == this->bucket().begin(); }
		constexpr bool is_hanging() const noexcept { return this->pointer() + 1 == this->bucket().end(); }
		constexpr bool is_climbing() const noexcept { return this->pointer() >= this->bucket().begin() && this->pointer() < this->bucket().end(); }

		constexpr bool is_at_root() const noexcept { return &this->bucket() == &this->wheel().root(); }
		constexpr bool is_at_head() const noexcept { return &this->bucket() == &this->wheel().root().prior(); }

	public:
		void write(std::string_view const& string) noexcept;

		void erase(Difference_Type diff);

		void move_up(Difference_Type steps);
		void move_down(Difference_Type steps);
		void move_right(Difference_Type steps);
		void move_left(Difference_Type steps);
 
		Bucket* locate_backwards(byte delimiter);
		Bucket* locate_forwards(byte delimeter);
	
	private:
		Text_Buffer& wheel_;
		Difference_Type offset_;
		Bucket* bucket_;
		byte const* pointer_; 
		Position position_;
		natptr column_;
	
	private:
		void drop_to(Bucket& bucket) noexcept;
		void jump_to(Bucket& bucket) noexcept;
		natptr compute_column() noexcept;
	};

	enum class Flag {
	
	};

public:
	constexpr Bucket const& root() const noexcept { return *this->root_; }

private:
	Bucket* root_;
	natptr height_;
};

using Text_Bucket = Text_Buffer::Bucket;
using Text_Cursor = Text_Buffer::Cursor;
using Text_Buffer_Flag = Text_Buffer::Flag;

}
