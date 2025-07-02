#include <print>

#include <UniT/length.hpp>
#include <UniT/mass.hpp>
#include <UniT/time.hpp>
#include <UniT/composed.hpp>

static_assert(UniT::is_same_group_v<
	UniT::UnitGroup<UniT::Second<float>, UniT::Meter<float>, UniT::Kilometer<float>>,
	UniT::UnitGroup<UniT::Second<float>, UniT::Kilometer<float>, UniT::Meter<float>>
>);


int main(int, char**) {
	using Momentum1 = UniT::Composed<
		UniT::UnitGroup<UniT::Kilogram<float>, UniT::Meter<float>>,
		UniT::UnitGroup<UniT::Second<float>>
	>;
	using Momentum2 = UniT::Composed<
		UniT::UnitGroup<UniT::Meter<float>, UniT::Kilogram<float>>,
		UniT::UnitGroup<UniT::Second<float>>
	>;

	static_assert(UniT::same_composed<Momentum1, Momentum2>);


	Momentum1 p1 {12.f};
	Momentum2 p2 {12.f};
	std::println("p1==p2 : {}", p1 == p2);

	return 0;
}
