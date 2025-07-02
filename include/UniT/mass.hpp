#pragma once

#include "UniT/formatting.hpp"
#include "UniT/single.hpp"


namespace UniT {
	namespace quantities {
		struct Mass {};
	}

	template <>
	struct quantity_symbol<quantities::Mass> {
		static constexpr std::string_view value {"kg"};
	};


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


	template <typename Rep>
	struct unit_symbol<Gigatonne<Rep>> {static constexpr auto value() noexcept -> std::string {return "Gt";}};
	template <typename Rep>
	struct unit_symbol<Megatonne<Rep>> {static constexpr auto value() noexcept -> std::string {return "Mt";}};
	template <typename Rep>
	struct unit_symbol<Kilotonne<Rep>> {static constexpr auto value() noexcept -> std::string {return "Kt";}};
	template <typename Rep>
	struct unit_symbol<Tonne<Rep>> {static constexpr auto value() noexcept -> std::string {return "t";}};
	template <typename Rep>
	struct unit_symbol<Gram<Rep>> {static constexpr auto value() noexcept -> std::string {return "g";}};
	template <typename Rep>
	struct unit_symbol<Milligram<Rep>> {static constexpr auto value() noexcept -> std::string {return "mg";}};
	template <typename Rep>
	struct unit_symbol<Microgram<Rep>> {static constexpr auto value() noexcept -> std::string {return "µg";}};
	template <typename Rep>
	struct unit_symbol<Nanogram<Rep>> {static constexpr auto value() noexcept -> std::string {return "ng";}};
	template <typename Rep>
	struct unit_symbol<Picogram<Rep>> {static constexpr auto value() noexcept -> std::string {return "pg";}};
}
