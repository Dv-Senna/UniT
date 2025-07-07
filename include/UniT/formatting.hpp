#pragma once

#include <algorithm>
#include <format>
#include <string>
#include <string_view>

#include "UniT/composed.hpp"
#include "UniT/single.hpp"
#include "UniT/utils.hpp"


namespace UniT {
	template <UniT::utils::quantity Quantity>
	struct quantity_symbol {
		static constexpr std::string_view value = "";
	};

	template <typename Quantity>
	constexpr auto quantity_symbol_v = quantity_symbol<Quantity>::value;


	template <UniT::utils::ratio Prefix>
	struct prefix_symbol {
		static constexpr std::string_view value = "";
	};

	template <typename Prefix>
	constexpr auto prefix_symbol_v = prefix_symbol<Prefix>::value;


	template <> struct prefix_symbol<std::exa>   {static constexpr std::string_view value = "E";};
	template <> struct prefix_symbol<std::peta>  {static constexpr std::string_view value = "P";};
	template <> struct prefix_symbol<std::tera>  {static constexpr std::string_view value = "T";};
	template <> struct prefix_symbol<std::giga>  {static constexpr std::string_view value = "G";};
	template <> struct prefix_symbol<std::mega>  {static constexpr std::string_view value = "M";};
	template <> struct prefix_symbol<std::kilo>  {static constexpr std::string_view value = "k";};
	template <> struct prefix_symbol<std::hecto> {static constexpr std::string_view value = "h";};
	template <> struct prefix_symbol<std::deca>  {static constexpr std::string_view value = "da";};
	template <> struct prefix_symbol<std::deci>  {static constexpr std::string_view value = "d";};
	template <> struct prefix_symbol<std::centi> {static constexpr std::string_view value = "c";};
	template <> struct prefix_symbol<std::milli> {static constexpr std::string_view value = "m";};
	template <> struct prefix_symbol<std::micro> {static constexpr std::string_view value = "µ";};
	template <> struct prefix_symbol<std::nano>  {static constexpr std::string_view value = "n";};
	template <> struct prefix_symbol<std::pico>  {static constexpr std::string_view value = "p";};
	template <> struct prefix_symbol<std::femto> {static constexpr std::string_view value = "f";};



	namespace __internals {
		template <typename T, typename Tuple, std::size_t I = 0, std::size_t N = std::tuple_size_v<Tuple>>
		struct get_tuple_type_index {
			static constexpr std::size_t value = std::same_as<T, std::tuple_element_t<I, Tuple>>
				? I : get_tuple_type_index<T, Tuple, I + 1, N>::value;
		};

		template <typename T, typename Tuple, std::size_t N>
		struct get_tuple_type_index<T, Tuple, N, N> {
			static constexpr std::size_t value = N;
		};

		template <typename T, typename Tuple>
		constexpr auto get_tuple_type_index_v = get_tuple_type_index<T, Tuple>::value;


		template <typename T, typename Tuple>
		struct append_to_tuple_if_not_exists {
			using type = std::conditional_t<get_tuple_type_index_v<T, Tuple> == std::tuple_size_v<Tuple>,
				UniT::__internals::merge_tuple_t<Tuple, std::tuple<T>>,
				Tuple
			>;
		};

		template <typename T, typename Tuple>
		using append_to_tuple_if_not_exists_t = typename append_to_tuple_if_not_exists<T, Tuple>::type;


		template <typename Tuple, std::size_t I = std::tuple_size_v<Tuple>>
		class tuple_type_occurence_map {
			private:
				using _Previous = tuple_type_occurence_map<Tuple, I - 1>;
				using _PreviousKeys = typename _Previous::keys;
				using _Current = std::tuple_element_t<I - 1, Tuple>;

			public:
				using keys = append_to_tuple_if_not_exists_t<_Current, _PreviousKeys>;
				static constexpr std::array<std::size_t, std::tuple_size_v<Tuple>> values = []() {
					constexpr std::size_t valueIndex {get_tuple_type_index_v<_Current, keys>};
					auto result {_Previous::values};
					++result[valueIndex];
					return result;
				} ();
		};

		template <typename Tuple>
		struct tuple_type_occurence_map<Tuple, 0uz> {
			using keys = std::tuple<>;
			static constexpr auto values = []() {
				std::array<std::size_t, std::tuple_size_v<Tuple>> result {};
				result.fill(0uz);
				return result;
			} ();
		};

		template <typename Tuple>
		using tuple_type_occurence_map_keys = typename tuple_type_occurence_map<Tuple>::keys;

		template <typename Tuple>
		constexpr auto tuple_type_occurence_map_values = tuple_type_occurence_map<Tuple>::values;


		namespace __tests {
			using Tuple = std::tuple<int, int, float, double, float, char, int, float, int, char>;
			static_assert(std::same_as<
				tuple_type_occurence_map_keys<Tuple>,
				std::tuple<int, float, double, char>
			>);
			static_assert(tuple_type_occurence_map_values<Tuple>[0] == 4);
			static_assert(tuple_type_occurence_map_values<Tuple>[1] == 3);
			static_assert(tuple_type_occurence_map_values<Tuple>[2] == 1);
			static_assert(tuple_type_occurence_map_values<Tuple>[3] == 2);
		}
	}


	template <UniT::unit T>
	struct unit_symbol {
		static constexpr auto value() noexcept -> std::string {return "";}
	};

	template <UniT::single T>
	struct unit_symbol<T> {
		static constexpr auto value() noexcept -> std::string {
			return std::string{prefix_symbol_v<UniT::get_single_prefix_t<T>>}
			+ std::string{quantity_symbol_v<UniT::get_single_quantity_t<T>>};
		}
	};

	template <UniT::composed _T>
	class unit_symbol<_T> {
		private:
			struct Info {
				std::string symbol;
				std::size_t count;
			};

			using T = UniT::reduce_composed_t<_T>;

			template <
				UniT::unit_group Group,
				std::size_t I = 0,
				std::size_t N = std::tuple_size_v<
					__internals::tuple_type_occurence_map_keys<UniT::__internals::group_to_tuple_t<Group>>
				>
			>
			static constexpr auto _unitGroupToInfos() noexcept -> std::array<Info, N> {
				using GroupAsTuple = UniT::__internals::group_to_tuple_t<Group>;

				if constexpr (I == N)
					return std::array<Info, N> {};
				else {
					auto infos {_unitGroupToInfos<Group, I + 1> ()};
					infos[I] = Info{
						.symbol = unit_symbol<std::tuple_element_t<I, __internals::tuple_type_occurence_map_keys<GroupAsTuple>>>::value(),
						.count = __internals::tuple_type_occurence_map_values<GroupAsTuple>[I]
					};
					return infos;
				}
			}

		public:
			static constexpr auto value() noexcept -> std::string {
				auto numInfos {_unitGroupToInfos<UniT::get_composed_num_t<T>> ()};
				auto denInfos {_unitGroupToInfos<UniT::get_composed_den_t<T>> ()};

				std::ranges::sort(numInfos, {}, &Info::symbol);
				std::ranges::sort(denInfos, {}, &Info::symbol);

				std::string result {};
				for (std::string_view prefix {""}; const auto &info : numInfos) {
					std::string superscript {info.count == 1 ? "" : std::format("^{}", info.count)};
					result += std::format("{}{}{}", prefix, info.symbol, superscript);
					prefix = "⋅";
				}

				for (std::string_view prefix {result.empty() ? "" : "⋅"}; const auto &info : denInfos) {
					result += std::format("{}{}^-{}", prefix, info.symbol, info.count);
					prefix = "⋅";
				}

				return result;
			}
	};


	template <UniT::unit T>
	constexpr auto toString(T value) noexcept {
		return std::format("{}{}", value.get(), unit_symbol<T>::value());
	}
}


template <UniT::unit T>
struct std::formatter<T> : std::formatter<std::string> {
	auto format(T value, std::format_context &ctx) const noexcept {
		return this->std::formatter<std::string>::format(toString(value), ctx);
	}
};
