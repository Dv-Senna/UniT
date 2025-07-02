#pragma once

#include <tuple>

#include "UniT/single.hpp"
#include "UniT/utils.hpp"


namespace UniT {
	namespace __internals {
		template <UniT::single ...Ts>
		struct is_valid_group : std::bool_constant<(std::same_as<
			UniT::get_single_rep_t<std::tuple_element_t<0, std::tuple<Ts...>>>,
			UniT::get_single_rep_t<Ts>
		> && ...)> {};

		template <>
		struct is_valid_group<> : std::true_type {};

		template <typename ...Ts>
		concept valid_group = is_valid_group<Ts...>::value;
	}


	template <UniT::single ...Ts>
	requires __internals::valid_group<Ts...>
	struct UnitGroup {};

	template <typename T>
	struct is_unit_group : std::false_type {};

	template <typename ...Ts>
	struct is_unit_group<UnitGroup<Ts...>> : std::true_type {};

	template <typename T>
	constexpr auto is_unit_group_v = is_unit_group<T>::value;

	template <typename T>
	concept unit_group = is_unit_group_v<T>;



	template <UniT::utils::quantity ...Ts>
	struct QuantityGroup {};

	template <typename T>
	struct is_quantity_group : std::false_type {};

	template <typename ...Ts>
	struct is_quantity_group<QuantityGroup<Ts...>> : std::true_type {};

	template <typename T>
	constexpr auto is_quantity_group_v = is_quantity_group<T>::value;

	template <typename T>
	concept quantity_group = is_quantity_group_v<T>;



	template <unit_group Group>
	struct flatten_unit_group_to_quantity_group;

	template <UniT::single ...Ts>
	struct flatten_unit_group_to_quantity_group<UnitGroup<Ts...>> {
		using type = QuantityGroup<UniT::get_single_quantity_t<Ts>...>;
	};

	template <typename Group>
	using flatten_unit_group_to_quantity_group_t = typename flatten_unit_group_to_quantity_group<Group>::type;


	template <typename Group>
	struct get_group_size;

	template <UniT::single ...Ts>
	struct get_group_size<UnitGroup<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

	template <UniT::utils::quantity ...Ts>
	struct get_group_size<QuantityGroup<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

	template <typename Group>
	constexpr auto get_group_size_v = get_group_size<Group>::value;


	template <unit_group Group>
	struct get_unit_group_rep;

	template <UniT::single ...Ts>
	struct get_unit_group_rep<UnitGroup<Ts...>> {
		using type = UniT::get_single_rep_t<std::tuple_element_t<0, std::tuple<Ts...>>>;
	};

	template <typename Group>
	using get_unit_group_rep_t = typename get_unit_group_rep<Group>::type;


	namespace __internals {
		template <typename Group>
		struct group_to_tuple;

		template <UniT::single ...Ts>
		struct group_to_tuple<UnitGroup<Ts...>> {
			using type = std::tuple<UniT::flatten_single_t<Ts>...>;
		};

		template <UniT::utils::quantity ...Ts>
		struct group_to_tuple<QuantityGroup<Ts...>> {
			using type = std::tuple<Ts...>;
		};

		template <typename Group>
		using group_to_tuple_t = typename group_to_tuple<Group>::type;


		template <typename Tuple>
		struct tuple_to_unit_group;

		template <UniT::single ...Ts>
		struct tuple_to_unit_group<std::tuple<Ts...>> {
			using type = UnitGroup<Ts...>;
		};

		template <typename Tuple>
		using tuple_to_unit_group_t = typename tuple_to_unit_group<Tuple>::type;


		template <typename Tuple>
		struct tuple_to_quantity_group;

		template <UniT::utils::quantity ...Ts>
		struct tuple_to_quantity_group<std::tuple<Ts...>> {
			using type = QuantityGroup<Ts...>;
		};

		template <typename Tuple>
		using tuple_to_quantity_group_t = typename tuple_to_quantity_group<Tuple>::type;



		template <typename E, typename Tuple, std::size_t I = 0, std::size_t N = std::tuple_size_v<Tuple>>
		struct count_element_in_tuple : std::integral_constant<std::size_t,
			count_element_in_tuple<E, Tuple, I+1, N>::value + (std::same_as<E, std::tuple_element_t<I, Tuple>> ? 1uz : 0uz)
		> {};

		template <typename E, typename Tuple, std::size_t N>
		struct count_element_in_tuple<E, Tuple, N, N> : std::integral_constant<std::size_t, 0uz> {};

		static_assert(count_element_in_tuple<int, std::tuple<float, char, double>>::value == 0);
		static_assert(count_element_in_tuple<int, std::tuple<float, char, int, double>>::value == 1);
		static_assert(count_element_in_tuple<int, std::tuple<float, int, int, char, double, int>>::value == 3);
		static_assert(count_element_in_tuple<int, std::tuple<int, float, int, int, char, double, int>>::value == 4);


		template <typename ...Tuples>
		struct merge_tuple {
			using type = decltype(std::tuple_cat(std::declval<Tuples> ()...));
		};

		template <typename ...Tuples>
		using merge_tuple_t = typename merge_tuple<Tuples...>::type;


		template <typename Element, typename Tuple>
		struct append_element_to_tuple {
			using type = merge_tuple_t<Tuple, std::tuple<Element>>;
		};

		template <typename Element, typename Tuple>
		using append_element_to_tuple_t = typename append_element_to_tuple<Element, Tuple>::type;


		template <typename Tuple>
		struct pop_first_element_of_tuple {
			using type = std::tuple<>;
		};

		template <typename First, typename ...Ts>
		struct pop_first_element_of_tuple<std::tuple<First, Ts...>> {
			using type = std::tuple<Ts...>;
		};

		template <typename Tuple>
		using pop_first_element_of_tuple_t = typename pop_first_element_of_tuple<Tuple>::type;


		template <typename Element, typename Tuple>
		struct remove_element_from_tuple {
			using type = std::conditional_t<std::same_as<Element, std::tuple_element_t<0, Tuple>>,
				pop_first_element_of_tuple_t<Tuple>,
				merge_tuple_t<
					std::tuple<std::tuple_element_t<0, Tuple>>,
					typename remove_element_from_tuple<Element, pop_first_element_of_tuple_t<Tuple>>::type
				>
			>;
		};

		template <typename Element>
		struct remove_element_from_tuple<Element, std::tuple<>> {
			using type = std::tuple<>;
		};


		template <typename Element, typename Tuple>
		using remove_element_from_tuple_t = typename remove_element_from_tuple<Element, Tuple>::type;

		static_assert(std::same_as<std::tuple<int, float, float>, remove_element_from_tuple_t<float, std::tuple<float, int, float, float>>>);
		static_assert(std::same_as<std::tuple<float, float, float>, remove_element_from_tuple_t<int, std::tuple<float, int, float, float>>>);


		template <typename Lhs, typename Rhs>
		struct substract_tuples {
			using type = typename substract_tuples<
				remove_element_from_tuple_t<std::tuple_element_t<0, Rhs>, Lhs>,
				pop_first_element_of_tuple_t<Rhs>
			>::type;
		};

		template <typename Rhs>
		struct substract_tuples<std::tuple<>, Rhs> {using type = std::tuple<>;};

		template <typename Lhs>
		struct substract_tuples<Lhs, std::tuple<>> {using type = Lhs;};

		template <>
		struct substract_tuples<std::tuple<>, std::tuple<>> {using type = std::tuple<>;};

		template <typename Lhs, typename Rhs>
		using substract_tuples_t = typename substract_tuples<Lhs, Rhs>::type;


		static_assert(std::same_as<
			substract_tuples_t<
				std::tuple<int, float, float, double, int, double, int>,
				std::tuple<float, int>
			>,
			std::tuple<float, double, int, double, int>
		>);
	}


	template <typename Element, typename Group>
	struct element_count_in_group;

	template <UniT::single Unit, UniT::single ...Ts>
	struct element_count_in_group<Unit, UnitGroup<Ts...>> : __internals::count_element_in_tuple<Unit, std::tuple<Ts...>> {};

	template <UniT::utils::quantity Quantity, UniT::utils::quantity ...Ts>
	struct element_count_in_group<Quantity, QuantityGroup<Ts...>> : __internals::count_element_in_tuple<Quantity, std::tuple<Ts...>> {};

	template <typename Element, typename Group>
	constexpr auto element_count_in_group_v = element_count_in_group<Element, Group>::value;



	/**
	 * @brief Compute `L\R := L\(L∩R)`
	 * */
	template <typename Lhs, typename Rhs>
	struct substract_groups;

	template <unit_group Lhs, unit_group Rhs>
	struct substract_groups<Lhs, Rhs> {
		using type = __internals::tuple_to_unit_group_t<__internals::substract_tuples_t<
			__internals::group_to_tuple_t<Lhs>,
			__internals::group_to_tuple_t<Rhs>
		>>;
	};

	template <quantity_group Lhs, quantity_group Rhs>
	struct substract_groups<Lhs, Rhs> {
		using type = __internals::tuple_to_quantity_group_t<__internals::substract_tuples_t<
			__internals::group_to_tuple_t<Lhs>,
			__internals::group_to_tuple_t<Rhs>
		>>;
	};

	template <typename Lhs, typename Rhs>
	using substract_groups_t = typename substract_groups<Lhs, Rhs>::type;


	template <typename Lhs, typename Rhs>
	struct is_same_group : std::bool_constant<get_group_size_v<Lhs> == get_group_size_v<Rhs>
		&& get_group_size_v<substract_groups_t<Lhs, Rhs>> == 0
	> {};

	template <typename Lhs, typename Rhs>
	constexpr auto is_same_group_v = is_same_group<Lhs, Rhs>::value;
}
