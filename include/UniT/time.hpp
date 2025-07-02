#pragma once

#include "UniT/single.hpp"


namespace UniT {
	namespace quantities {
		struct Time {};
	}

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
}
