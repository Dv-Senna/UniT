#pragma once

#include <concepts>
#include <ratio>
#include <type_traits>


namespace UniT::utils {
	template <typename T>
	struct is_ratio : std::false_type {};

	template <std::intmax_t NUM, std::intmax_t DEN>
	struct is_ratio<::std::ratio<NUM, DEN>> : std::true_type {};

	template <typename T>
	constexpr auto is_ratio_v = is_ratio<T>::value;

	template <typename T>
	concept ratio = is_ratio_v<T>;


	template <typename T>
	concept arithmetic = std::is_arithmetic_v<T>;

	template <typename T>
	concept empty = std::is_empty_v<T>;


	template <typename T>
	concept quantity = empty<T>;

	template <typename T>
	concept rep = arithmetic<T>;
}
