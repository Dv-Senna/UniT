#pragma once

#include "UniT/group.hpp"
#include "UniT/single.hpp"
#include "UniT/utils.hpp"


namespace UniT {
	template <UniT::utils::rep Rep, UniT::unit_group Num, UniT::unit_group Den>
	requires UniT::is_unit_group_of_rep_v<Rep, Num> && UniT::is_unit_group_of_rep_v<Rep, Den>
	class Composed;


	template <typename T>
	struct is_composed : std::false_type {};

	template <typename Rep, typename Num, typename Den>
	struct is_composed<Composed<Rep, Num, Den>> : std::true_type {};

	template <typename T>
	constexpr auto is_composed_v = is_composed<T>::value;

	template <typename T>
	concept composed = is_composed_v<T>;


	template <composed Lhs, composed Rhs>
	struct is_same_composed : std::false_type {};

	template <typename Rep, UniT::unit_group LNum, UniT::unit_group LDen, UniT::unit_group RNum, UniT::unit_group RDen>
	struct is_same_composed<Composed<Rep, LNum, LDen>, Composed<Rep, RNum, RDen>> {
		static constexpr bool value = UniT::is_same_group_v<
			UniT::substract_groups_t<LNum, LDen>,
			UniT::substract_groups_t<RNum, RDen>
		> && UniT::is_same_group_v<
			UniT::substract_groups_t<LDen, LNum>,
			UniT::substract_groups_t<RDen, RNum>
		>;
		using value_type = decltype(value);
	};

	template <typename Lhs, typename Rhs>
	constexpr auto is_same_composed_v = is_same_composed<Lhs, Rhs>::value;

	template <typename Lhs, typename Rhs>
	concept same_composed = is_same_composed<Lhs, Rhs>::value;



	template <UniT::utils::rep Rep, UniT::unit_group Num, UniT::unit_group Den>
	requires UniT::is_unit_group_of_rep_v<Rep, Num> && UniT::is_unit_group_of_rep_v<Rep, Den>
	class Composed final {
		using This = Composed<Rep, Num, Den>;

		public:
			[[gnu::always_inline]]
			constexpr Composed() noexcept = default;
			[[gnu::always_inline]]
			constexpr ~Composed() = default;

			[[gnu::always_inline]]
			constexpr Composed(const same_composed<This> auto &other) noexcept : m_data {other.get()} {}
			[[gnu::always_inline]]
			constexpr auto operator=(const same_composed<This> auto &other) noexcept -> This& {
				m_data = other.get();
				return *this;
			}

			[[gnu::always_inline]]
			constexpr Composed(Rep data) noexcept : m_data {data} {}

			template <typename Other>
			requires same_composed<This, std::remove_cvref_t<Other>>
			[[gnu::always_inline]]
			constexpr auto operator+=(Other &&other) noexcept -> This& {m_data += other.get(); return *this;}
			template <typename Other>
			requires same_composed<This, std::remove_cvref_t<Other>>
			[[gnu::always_inline]]
			constexpr auto operator-=(Other &&other) noexcept -> This& {m_data -= other.get(); return *this;}

			[[gnu::always_inline]]
			constexpr auto operator+(auto &&other) const noexcept -> This {auto tmp {*this}; return tmp += other;}
			[[gnu::always_inline]]
			constexpr auto operator-(auto &&other) const noexcept -> This {auto tmp {*this}; return tmp -= other;}

			[[gnu::always_inline]]
			constexpr auto operator*=(Rep value) noexcept -> This& {m_data *= value; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator/=(Rep value) noexcept -> This& {m_data /= value; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator*(Rep value) const noexcept -> This {auto tmp {*this}; return tmp *= value;}
			[[gnu::always_inline]]
			constexpr auto operator/(Rep value) const noexcept -> This {auto tmp {*this}; return tmp /= value;}

			[[gnu::always_inline]]
			constexpr auto operator-() const noexcept -> This requires (!std::is_unsigned_v<Rep>) {
				auto tmp {*this};
				tmp.m_data = -tmp.m_data;
				return tmp;
			}

			[[gnu::always_inline]]
			constexpr auto get() const noexcept -> Rep {return m_data;}


			template <same_composed<This> Other>
			[[gnu::always_inline]]
			constexpr operator Other() const noexcept {return Other{m_data};}

		private:
			Rep m_data;
	};


	template <composed Lhs, composed Rhs>
	requires same_composed<Lhs, Rhs>
	[[gnu::always_inline]]
	constexpr auto operator==(const Lhs &lhs, const Rhs &rhs) noexcept -> bool {return lhs.get() == rhs.get();}
	template <composed Lhs, composed Rhs>
	requires same_composed<Lhs, Rhs>
	[[gnu::always_inline]]
	constexpr auto operator<=>(const Lhs &lhs, const Rhs &rhs) noexcept -> bool {return lhs.get() <=> rhs.get();}



	template <composed T>
	struct get_composed_num;

	template <typename Rep, UniT::unit_group Num, UniT::unit_group Den>
	struct get_composed_num<Composed<Rep, Num, Den>> {
		using type = Num;
	};

	template <typename T>
	using get_composed_num_t = typename get_composed_num<T>::type;


	template <composed T>
	struct get_composed_den;

	template <typename Rep, UniT::unit_group Num, UniT::unit_group Den>
	struct get_composed_den<Composed<Rep, Num, Den>> {
		using type = Den;
	};

	template <typename T>
	using get_composed_den_t = typename get_composed_den<T>::type;


	template <composed T>
	struct get_composed_rep;

	template <typename Rep, UniT::unit_group Num, UniT::unit_group Den>
	struct get_composed_rep<Composed<Rep, Num, Den>> {
		using type = Rep;
	};

	template <typename T>
	using get_composed_rep_t = typename get_composed_rep<T>::type;



	template <composed T>
	struct reduce_composed {
		using type = Composed<
			get_composed_rep_t<T>,
			UniT::substract_groups_t<get_composed_num_t<T>, get_composed_den_t<T>>,
			UniT::substract_groups_t<get_composed_den_t<T>, get_composed_num_t<T>>
		>;
	};

	template <typename T>
	using reduce_composed_t = typename reduce_composed<T>::type;


	template <composed Lhs, composed Rhs>
	requires std::same_as<get_composed_rep_t<Lhs>, get_composed_rep_t<Rhs>>
	struct merge_composed {
		using type = reduce_composed_t<Composed<
			get_composed_rep_t<Lhs>,
			UniT::merge_group_t<get_composed_num_t<Lhs>, get_composed_num_t<Rhs>>,
			UniT::merge_group_t<get_composed_den_t<Lhs>, get_composed_den_t<Rhs>>
		>>;
	};

	template <typename Lhs, typename Rhs>
	using merge_composed_t = typename merge_composed<Lhs, Rhs>::type;


	template <composed T>
	struct inverse_composed {
		using type = reduce_composed_t<Composed<
			get_composed_rep_t<T>,
			get_composed_den_t<T>,
			get_composed_num_t<T>
		>>;
	};

	template <typename T>
	using inverse_composed_t = typename inverse_composed<T>::type;


	template <UniT::single T>
	struct single_as_composed {
		using type = Composed<UniT::get_single_rep_t<T>, UniT::UnitGroup<T>, UniT::UnitGroup<>>;
	};

	template <typename T>
	using single_as_composed_t = typename single_as_composed<T>::type;


	template <composed Lhs, composed Rhs>
	[[gnu::always_inline]]
	constexpr auto operator*(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<Lhs, Rhs> {lhs.get() * rhs.get()};
	}

	template<composed Lhs, composed Rhs>
	[[gnu::always_inline]]
	constexpr auto operator/(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<Lhs, inverse_composed_t<Rhs>> {lhs.get() / rhs.get()};
	};

	template <composed T>
	[[gnu::always_inline]]
	constexpr auto operator/(get_composed_rep_t<T> factor, T quantity) noexcept {
		return inverse_composed_t<T> {factor / quantity.get()};
	};


	template <UniT::single Lhs, UniT::single Rhs>
	[[gnu::always_inline]]
	constexpr auto operator*(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<single_as_composed_t<Lhs>, single_as_composed_t<Rhs>> {lhs.get() * rhs.get()};
	}

	template <UniT::single Lhs, UniT::single Rhs>
	[[gnu::always_inline]]
	constexpr auto operator/(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<single_as_composed_t<Lhs>, inverse_composed_t<single_as_composed_t<Rhs>>> {lhs.get() / rhs.get()};
	}

	template <UniT::single T>
	[[gnu::always_inline]]
	constexpr auto operator/(get_single_rep_t<T> factor, T quantity) noexcept {
		return inverse_composed_t<single_as_composed_t<T>> {factor / quantity.get()};
	};


	template <UniT::composed Lhs, UniT::single Rhs>
	[[gnu::always_inline]]
	constexpr auto operator*(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<Lhs, single_as_composed_t<Rhs>> {lhs.get() * rhs.get()};
	}

	template <UniT::single Lhs, composed Rhs>
	[[gnu::always_inline]]
	constexpr auto operator*(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<single_as_composed_t<Lhs>, Rhs> {lhs.get() * rhs.get()};
	}


	template <UniT::composed Lhs, UniT::single Rhs>
	[[gnu::always_inline]]
	constexpr auto operator/(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<Lhs, inverse_composed_t<single_as_composed_t<Rhs>>> {lhs.get() / rhs.get()};
	}

	template <UniT::single Lhs, UniT::composed Rhs>
	[[gnu::always_inline]]
	constexpr auto operator/(Lhs lhs, Rhs rhs) noexcept {
		return merge_composed_t<single_as_composed_t<Lhs>, inverse_composed_t<Rhs>> {lhs.get() / rhs.get()};
	}


	template <typename T>
	concept unit = UniT::single<T> || composed<T>;
}
