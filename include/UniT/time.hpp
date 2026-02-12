#pragma once

#ifndef UNIT_TYPE_ONLY
	#include "UniT/formatting.hpp"
#endif
#include "UniT/single.hpp"


namespace UniT {
	namespace quantities {
		struct Time {};
	}

#ifndef UNIT_TYPE_ONLY
	template <>
	struct quantity_symbol<quantities::Time> {
		static constexpr std::string_view value {"s"};
	};
#endif


	template <typename Rep, typename Prefix = std::ratio<1, 1>>
	using Duration = UniT::Single<quantities::Time, Rep, Prefix>;


	template <typename Rep> using UniverseAge     = Duration<Rep, std::ratio<436'117'077'000'000'000, 1>>;
	template <typename Rep> using JulianYear      = Duration<Rep, std::ratio<31'557'600, 1>>;
	template <typename Rep> using Day             = Duration<Rep, std::ratio<86'400, 1>>;
	template <typename Rep> using Hour            = Duration<Rep, std::ratio<3600, 1>>;
	template <typename Rep> using Minute          = Duration<Rep, std::ratio<60, 1>>;
	template <typename Rep> using Second          = Duration<Rep>;
	template <typename Rep> using SecondTenth     = Duration<Rep, std::ratio<1, 10>>;
	template <typename Rep> using SecondHundredth = Duration<Rep, std::ratio<1, 100>>;
	template <typename Rep> using Millisecond     = Duration<Rep, std::milli>;
	template <typename Rep> using Microsecond     = Duration<Rep, std::micro>;
	template <typename Rep> using Nanosecond      = Duration<Rep, std::nano>;
	template <typename Rep> using Picosecond      = Duration<Rep, std::pico>;
	template <typename Rep> using Femtosecond     = Duration<Rep, std::femto>;


#ifndef UNIT_TYPE_ONLY
	template <typename Rep>
	struct unit_symbol<UniverseAge<Rep>> {static constexpr auto value() noexcept -> std::string {return "ua";}};
	template <typename Rep>
	struct unit_symbol<JulianYear<Rep>> {static constexpr auto value() noexcept -> std::string {return "year";}};
	template <typename Rep>
	struct unit_symbol<Day<Rep>> {static constexpr auto value() noexcept -> std::string {return "day";}};
	template <typename Rep>
	struct unit_symbol<Hour<Rep>> {static constexpr auto value() noexcept -> std::string {return "h";}};
	template <typename Rep>
	struct unit_symbol<Minute<Rep>> {static constexpr auto value() noexcept -> std::string {return "min";}};
	template <typename Rep>
	struct unit_symbol<SecondTenth<Rep>> {static constexpr auto value() noexcept -> std::string {return "s10th";}};
	template <typename Rep>
	struct unit_symbol<SecondHundredth<Rep>> {static constexpr auto value() noexcept -> std::string {return "s100th";}};
#endif
}
