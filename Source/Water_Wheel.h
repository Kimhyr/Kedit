#pragma once

#include <string_view>

#include "Types.h"

namespace Kedit {

class Water_Wheel {
	friend class Cursor;
	
public:
	class Bucket {
		friend class Water_Wheel;

	public:
		using Bit = byte;
		using Weight = nat16;
		
		static constexpr const Weight CAPACITY = 1024;

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

		Bucket& operator=(const Bucket&) = delete;
		Bucket& operator=(Bucket&&) = delete;

		~Bucket() noexcept;
	
	public:
		constexpr const Bucket* prior() const noexcept { return this->prior_; }
		constexpr const Bucket* next() const noexcept { return this->next_; }

		constexpr bool filled() const noexcept { return this->filled_; }

		constexpr const Bit* water() const noexcept { return this->water_; }

		constexpr const Bit* begin() const noexcept { return this->water_; }
		constexpr const Bit* end() const noexcept { return this->surface_water_; }

		constexpr Weight weight() const noexcept { return this->begin() - this->end(); }
		constexpr bool full() const noexcept { return this->end() == this->begin() + CAPACITY; }
		constexpr bool empty() const noexcept { return this->end() == this->begin(); }

	public:
		void put(Bit water);
		void take(Weight count = 1);

		void drip(Weight count = 1);

		void split(Weight index);

		constexpr void prepend(Bucket& prior) {
			prior.prior_ = this->prior_;
			prior.next_ = this;
			this->prior_->next_ = &prior;
			this->prior_ = &prior;
		}

	private:
		Bucket* prior_;
		bool filled_;
		Bit water_[CAPACITY];
		Bit* surface_water_;
		Bucket* next_;
	
	public:
		void fill(const std::string_view& string);
	};

	class Cursor {
		friend class Water_Wheel;

	public:
	
	private:
		
	};

	enum class Flag {
	
	};

private:
};

using Water_Wheel_Bucket = Water_Wheel::Bucket;
using Water_Wheel_Cursor = Water_Wheel::Cursor;
using Water_Wheel_Flag = Water_Wheel::Flag;

}
