#pragma once

#include "UniT/utils.hpp"


namespace UniT {
	template <UniT::utils::quantity Quantity, UniT::utils::rep Rep, UniT::utils::ratio Prefix>
	class Single final {
		using This = Single<Quantity, Rep, Prefix>;

		public:
			template <typename Rep2, typename Prefix2>
			using Rebind = Single<Quantity, Rep2, Prefix2>;

			[[gnu::always_inline]]
			constexpr Single() noexcept = default;
			[[gnu::always_inline]]
			constexpr Single(const This&) noexcept = default;
			[[gnu::always_inline]]
			constexpr auto operator=(const This&) noexcept -> This& = default;
			[[gnu::always_inline]]
			constexpr Single(This&&) noexcept = default;
			[[gnu::always_inline]]
			constexpr auto operator=(This&&) noexcept -> This& = default;
			[[gnu::always_inline]]
			constexpr ~Single() = default;

			[[gnu::always_inline]]
			constexpr explicit Single(Rep data) noexcept : m_data {data} {}

			[[gnu::always_inline]]
			constexpr auto operator==(const This&) const noexcept -> bool = default;
			[[gnu::always_inline]]
			constexpr auto operator<=>(const This&) const noexcept = default;

			[[gnu::always_inline]]
			constexpr auto operator+=(const This &other) noexcept -> This& {m_data += other; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator-=(const This &other) noexcept -> This& {m_data += other; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator+(const This &other) const noexcept -> This {auto tmp {*this}; return tmp += other;}
			[[gnu::always_inline]]
			constexpr auto operator-(const This &other) const noexcept -> This {auto tmp {*this}; return tmp -= other;}

			[[gnu::always_inline]]
			constexpr auto operator*=(UniT::utils::arithmetic auto factor) noexcept -> This& {m_data *= factor; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator/=(UniT::utils::arithmetic auto factor) noexcept -> This& {m_data /= factor; return *this;}
			[[gnu::always_inline]]
			constexpr auto operator*(UniT::utils::arithmetic auto factor) const noexcept -> This {auto tmp {*this}; return tmp *= factor;}
			[[gnu::always_inline]]
			constexpr auto operator/(UniT::utils::arithmetic auto factor) const noexcept -> This {auto tmp {*this}; return tmp /= factor;}

			[[gnu::always_inline]]
			constexpr auto operator-() const noexcept -> This requires (!std::is_unsigned_v<Rep>) {
				auto tmp {*this};
				tmp.m_data = -tmp.m_data;
				return tmp;
			};

			template <typename Rep2, typename Prefix2>
			[[gnu::always_inline]]
			explicit constexpr operator Rebind<Rep2, Prefix2> () const noexcept {
				using ConvPrefix = std::ratio_divide<Prefix, Prefix>;
				return Rebind<Rep2, Prefix2> {static_cast<Rep2> ((m_data * ConvPrefix::num) / ConvPrefix::den)};
			};

			[[gnu::always_inline]]
			constexpr auto get() const noexcept -> Rep {return m_data;}


		private:
			Rep m_data;
	};


	template <typename T>
	struct is_single : std::false_type {};

	template <typename Quantity, typename Rep, typename Prefix>
	struct is_single<Single<Quantity, Rep, Prefix>> : std::true_type {};

	template <typename T>
	constexpr auto is_single_v = is_single<T>::value;

	template <typename T>
	concept single = is_single_v<T>;


	[[gnu::always_inline]]
	constexpr auto operator*(UniT::utils::arithmetic auto factor, single auto single) noexcept {return single *= factor;}


	template <single Lhs, single Rhs>
	struct is_single_same_quantity : std::false_type {};

	template <typename Quantity, typename Rep1, typename Rep2, typename Prefix1, typename Prefix2>
	struct is_single_same_quantity<Single<Quantity, Rep1, Prefix1>, Single<Quantity, Rep2, Prefix2>> : std::true_type {};

	template <typename Lhs, typename Rhs>
	constexpr auto is_single_same_quantity_v = is_single_same_quantity<Lhs, Rhs>::value;

	template <typename Lhs, typename Rhs>
	concept single_same_quantity = is_single_same_quantity_v<Lhs, Rhs>;


	template <single T, UniT::utils::quantity Quantity>
	struct is_single_of_quantity : std::false_type {};

	template <typename Quantity, typename Rep, typename Prefix>
	struct is_single_of_quantity<Single<Quantity, Rep, Prefix>, Quantity> : std::true_type {};

	template <typename T, typename Quantity>
	constexpr auto is_single_of_quantity_v = is_single_of_quantity<T, Quantity>::value;

	template <typename T, typename Quantity>
	concept single_of_quantity = is_single_of_quantity_v<T, Quantity>;


	template <typename T>
	struct get_single_quantity;

	template <typename Quantity, typename Rep, typename Prefix>
	struct get_single_quantity<Single<Quantity, Rep, Prefix>> {using type = Quantity;};

	template <typename T>
	using get_single_quantity_t = typename get_single_quantity<T>::type;


	template <typename T>
	struct get_single_rep;

	template <typename Quantity, typename Rep, typename Prefix>
	struct get_single_rep<Single<Quantity, Rep, Prefix>> {using type = Rep;};

	template <typename T>
	using get_single_rep_t = typename get_single_rep<T>::type;


	template <typename T>
	struct get_single_prefix;

	template <typename Quantity, typename Rep, typename Prefix>
	struct get_single_prefix<Single<Quantity, Rep, Prefix>> {using type = Prefix;};

	template <typename T>
	using get_single_prefix_t = typename get_single_prefix<T>::type;


	template <single T>
	struct flatten_single;

	template <typename Quantity, typename Rep, typename Prefix>
	struct flatten_single<Single<Quantity, Rep, Prefix>> {
		using type = Single<Quantity, Rep, std::ratio<Prefix::num, Prefix::den>>;
	};

	template <typename T>
	using flatten_single_t = typename flatten_single<T>::type;
}
