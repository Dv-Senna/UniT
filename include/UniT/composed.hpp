#pragma once

#include "UniT/group.hpp"
#include "UniT/single.hpp"
#include "UniT/utils.hpp"


namespace UniT {
	template <UniT::unit_group Num, UniT::unit_group Den>
	requires std::same_as<UniT::get_unit_group_rep_t<Num>, UniT::get_unit_group_rep_t<Den>>
	class Composed;


	template <typename T>
	struct is_composed : std::false_type {};

	template <typename Num, typename Den>
	struct is_composed<Composed<Num, Den>> : std::true_type {};

	template <typename T>
	constexpr auto is_composed_v = is_composed<T>::value;

	template <typename T>
	concept composed = is_composed_v<T>;


	template <composed Lhs, composed Rhs>
	struct is_same_composed : std::false_type {};

	template <UniT::unit_group LNum, UniT::unit_group LDen, UniT::unit_group RNum, UniT::unit_group RDen>
	struct is_same_composed<Composed<LNum, LDen>, Composed<RNum, RDen>> {
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
	concept same_composed = is_same_composed_v<Lhs, Rhs>;



	template <UniT::unit_group Num, UniT::unit_group Den>
	requires std::same_as<UniT::get_unit_group_rep_t<Num>, UniT::get_unit_group_rep_t<Den>>
	class Composed final {
		using Rep = UniT::get_unit_group_rep_t<Num>;
		using This = Composed<Num, Den>;

		static_assert(std::same_as<Rep, float>);

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
}
