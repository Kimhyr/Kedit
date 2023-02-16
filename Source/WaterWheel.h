#pragma once

#include <string_view>
#include <stdexcept>
#include <algorithm>

#include "Types.h"

namespace Kedit {


class WaterWheel {
	friend class Bucket;
	friend class Cursor;

public:
	class Bucket {
		friend class WaterWheel;
	
	public:
		using WeightType = nat16;
		using DifferenceType = WeightType;
	
		
		static constexpr const WeightType CAPACITY = 1024;
	
	public:
		constexpr Bucket(bool filled) noexcept
			: prior_(this), filled_(filled), next_(this) {} 
		constexpr Bucket(Bucket& prior, bool filled)
			: prior_(&prior), filled_(filled), next_(prior.next_) {
			prior.next_->prior_ = this;
			prior.next_ = this;
		}
		Bucket(const Bucket&) = delete;
		Bucket(Bucket&&) = delete;
	
		Bucket& operator=(Bucket const&) = delete;
		Bucket& operator=(Bucket&&) = delete;
	
		~Bucket() = default;
	
	public:
		constexpr Bucket const& prior() const noexcept { return *this->prior_; }
		constexpr Bucket const& next() const noexcept { return *this->next_; }
	
		constexpr bool filled() const noexcept { return this->filled_; }
	
		constexpr const byte* water() const noexcept { return this->water_; }
	
		constexpr const byte* begin() const noexcept { return this->water_; }
		constexpr const byte* end() const noexcept { return this->surface_; }
	
		constexpr WeightType weight() const noexcept { return this->begin() - this->end(); }
		constexpr bool is_full() const noexcept { return this->end() == this->begin() + CAPACITY; }
		constexpr bool is_empty() const noexcept { return this->end() == this->begin(); }
	
		constexpr byte& operator[](intptr index) noexcept { return this->water_[index]; }
		constexpr byte& at(WeightType index) {
			if (index >= this->weight())
				throw std::out_of_range(__FUNCTION__);
			return this->water_[index];
		}
	
	public:
		void put(byte water);
		
		void take(DifferenceType count = 1);
		void drip(DifferenceType count = 1);
	
		Bucket& split(DifferenceType at);
	
		constexpr void prepend(Bucket& prior) noexcept {
			prior.prior_ = this->prior_;
			prior.next_ = this;
			this->prior_->next_ = &prior;
			this->prior_ = &prior;
		}
	
	private:
		Bucket* prior_;
		bool filled_;
		byte water_[CAPACITY];
		byte* surface_;
		Bucket* next_;
	};

	class Cursor {
		friend class WaterWheel;

	public:
		using DifferenceType = natptr;

	public:
		constexpr Cursor(WaterWheel& wheel) noexcept
			: wheel_(wheel), offset_(0), bucket_(wheel.root_), pointer_(wheel.root_->begin() - 1),
			  column_(0) {}

		~Cursor() = default;

	public:
		constexpr const WaterWheel& wheel() const noexcept { return this->wheel_; }

		constexpr Bucket const& bucket() const noexcept { return *this->bucket_; }
		constexpr byte const* pointer() const noexcept { return this->pointer_; }

		constexpr Position const& position() const noexcept { return this->position_; }
		constexpr DifferenceType column() const noexcept { return this->column_; }

		constexpr DifferenceType offset() const noexcept { return this->offset_;}
		constexpr DifferenceType depth() const noexcept { return this->pointer() - this->bucket().begin(); }

		constexpr byte i_dont_know_what_to_name_this_thing() const noexcept { return this->bucket().end()[-1]; }

		constexpr bool is_holding() const noexcept { return this->pointer() + 1 == this->bucket().begin(); }
		constexpr bool is_resting() const noexcept { return this->pointer() == this->bucket().begin(); }
		constexpr bool is_hanging() const noexcept { return this->pointer() + 1 == this->bucket().end(); }
		constexpr bool is_climbing() const noexcept { return this->pointer() >= this->bucket().begin() && this->pointer() < this->bucket().end(); }

		constexpr bool is_at_root() const noexcept { return &this->bucket() == &this->wheel().root(); }
		constexpr bool is_at_head() const noexcept { return &this->bucket() == &this->wheel().root().prior(); }

	public:
		void write(std::string_view const& string) noexcept;

		void erase(DifferenceType diff);

		void move_up(DifferenceType steps);
		void move_down(DifferenceType steps);
		void move_right(DifferenceType steps);
		void move_left(DifferenceType steps);
 
		Bucket* locate_backwards(byte delimiter);
		Bucket* locate_forwards(byte delimeter);
	
	private:
		WaterWheel& wheel_;
		DifferenceType offset_;
		Bucket* bucket_;
		byte const* pointer_; 
		Position position_;
		natptr column_;
	
	private:
		void drop_to(Bucket& bucket) noexcept;
		void jump_to(Bucket& bucket) noexcept;
	};

	enum class Flag {
	
	};

public:
	constexpr Bucket const& root() const noexcept { return *this->root_; }

private:
	Bucket* root_;
	natptr height_;
};

using WaterBucket = WaterWheel::Bucket;
using WaterWheelCursor = WaterWheel::Cursor;
using WaterWheelFlag = WaterWheel::Flag;

}
