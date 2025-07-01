#pragma once

#include "UniT/single.hpp"


namespace UniT {
	namespace quantities {
		struct Length {};
	}

	template <typename Rep, typename Prefix = std::ratio<1, 1>>
	using Distance = UniT::Single<quantities::Length, Rep, Prefix>;


	template <typename Rep> using Parsec           = Distance<Rep, std::ratio<30'856'775'814'913'673, 1>>;
	template <typename Rep> using LightYear        = Distance<Rep, std::ratio<9'460'730'472'580'800, 1>>;
	template <typename Rep> using AstronomicalUnit = Distance<Rep, std::ratio<149'597'870'700, 1>>;
	template <typename Rep> using Kilometer        = Distance<Rep, std::kilo>;
	template <typename Rep> using Hectometer       = Distance<Rep, std::hecto>;
	template <typename Rep> using Decameter        = Distance<Rep, std::deca>;
	template <typename Rep> using Meter            = Distance<Rep>;
	template <typename Rep> using Decimeter        = Distance<Rep, std::deci>;
	template <typename Rep> using Centimeter       = Distance<Rep, std::centi>;
	template <typename Rep> using Millimeter       = Distance<Rep, std::milli>;
	template <typename Rep> using Micrometer       = Distance<Rep, std::micro>;
	template <typename Rep> using Nanometer        = Distance<Rep, std::nano>;
	template <typename Rep> using Angström         = Distance<Rep, std::ratio<1, 10'000'000'000>>;
	template <typename Rep> using Picometer        = Distance<Rep, std::pico>;
	template <typename Rep> using Femtometer       = Distance<Rep, std::femto>;


	namespace freedom {
		template <typename Rep> using AverageMcdonaldDistance = Distance<Rep, std::ratio<51'499, 10>>;
		template <typename Rep> using BigMacWidth = Distance<Rep, std::ratio<2, 23>>;
		template <typename Rep> using FootballField = Distance<Rep, std::ratio<9144, 100>>;
		template <typename Rep> using TankOfGas = Distance<Rep, std::ratio<483'000, 1>>;
	}

	namespace forbidden {
		template <typename Rep> using Twip = Distance<Rep, std::ratio<127, 7'200'000>>;
		template <typename Rep> using Point = Distance<Rep, std::ratio<127, 360'000>>;
		template <typename Rep> using Inch = Distance<Rep, std::ratio<254, 10'000>>;
		template <typename Rep> using Foot = Distance<Rep, std::ratio<3048, 1000>>;
		template <typename Rep> using Yard = Distance<Rep, std::ratio<9144, 10>>;
		template <typename Rep> using Mile = Distance<Rep, std::ratio<1'609'344, 1000>>;
	}
}
