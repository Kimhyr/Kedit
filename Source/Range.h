#pragma once

namespace Kedit {

template<typename Road_T, Road_T* begin_T, const Road_T* end_T>
class Range {
public:
	typedef Road_T RoadType;

public:
	constexpr RoadType* begin() noexcept { return begin_T; }
	constexpr const RoadType* end() noexcept { return end_T; }
};

}
