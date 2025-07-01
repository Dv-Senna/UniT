#pragma once

#include "UniT/single.hpp"


namespace UniT {
	namespace quantities {
		struct Mass {};
	}

	template <typename Rep, typename Prefix = std::ratio<1, 1>>
	using Mass = UniT::Single<quantities::Mass, Rep, Prefix>;


	template <typename Rep> using Gigatonne = Mass<Rep, std::tera>;
	template <typename Rep> using Megatonne = Mass<Rep, std::giga>;
	template <typename Rep> using Kilotonne = Mass<Rep, std::mega>;
	template <typename Rep> using Tonne     = Mass<Rep, std::kilo>;
	template <typename Rep> using Kilogram  = Mass<Rep>;
	template <typename Rep> using Gram      = Mass<Rep, std::milli>;
	template <typename Rep> using Milligram = Mass<Rep, std::micro>;
	template <typename Rep> using Microgram = Mass<Rep, std::nano>;
	template <typename Rep> using Nanogram  = Mass<Rep, std::pico>;
	template <typename Rep> using Picogram  = Mass<Rep, std::femto>;
}
