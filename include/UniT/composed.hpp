#pragma once

#include "UniT/single.hpp"
#include "UniT/unitGroup.hpp"
#include "UniT/utils.hpp"


namespace UniT {
	template <unit_group Num, unit_group Den>
	class Composed;


	template <typename T>
	struct is_composed : std::false_type {};

	template <typename Num, typename Den>
	struct is_composed<Composed<Num, Den>> : std::true_type {};

	template <typename T>
	constexpr auto is_composed_v = is_composed<T>::value;

	template <typename T>
	concept composed = is_composed_v<T>;



	template <unit_group Num, unit_group Den>
	class Composed final {
		
	};
}
