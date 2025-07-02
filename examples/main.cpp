#include <print>

#include <UniT/length.hpp>
#include <UniT/mass.hpp>
#include <UniT/time.hpp>
#include <UniT/composed.hpp>
#include <UniT/formatting.hpp>

static_assert(UniT::is_same_group_v<
	UniT::UnitGroup<UniT::Second<float>, UniT::Meter<float>, UniT::Kilometer<float>>,
	UniT::UnitGroup<UniT::Second<float>, UniT::Kilometer<float>, UniT::Meter<float>>
>);


int main(int, char**) {
	using Momentum1 = UniT::Composed<
		float,
		UniT::UnitGroup<UniT::Kilogram<float>, UniT::Meter<float>>,
		UniT::UnitGroup<UniT::Second<float>>
	>;
	using Momentum2 = UniT::Composed<
		float,
		UniT::UnitGroup<UniT::Meter<float>, UniT::Kilogram<float>>,
		UniT::UnitGroup<UniT::Second<float>>
	>;

	using KgPerS = UniT::Composed<
		float,
		UniT::UnitGroup<UniT::Kilogram<float>>,
		UniT::UnitGroup<UniT::Second<float>>
	>;

	static_assert(UniT::same_composed<Momentum1, Momentum2>);


	UniT::Second duration {10.f};
	UniT::Meter distance {120.f};
	UniT::freedom::BigMacWidth<float> distance2 {1.f};
	UniT::Kilogram mass {1.f};

	auto p1 {mass * distance / duration};
	auto p2 {distance * mass / duration / distance2};
	auto p3 {UniT::unitCast<KgPerS> (p2)};
	auto m2 {UniT::unitCast<UniT::Kilogram<float>> (p3 * duration)};

	//static_assert(std::same_as<Momentum1, decltype(p1)>);
	//static_assert(std::same_as<Momentum2, decltype(p2)>);
	//static_assert(UniT::same_composed<decltype(p1), decltype(p2)>);

	std::println("duration : {}", duration);
	std::println("distance : {}", distance);
	std::println("mass : {}", mass);
	std::println("p1 : {}", p1);
	std::println("p2 : {}", p2);
	std::println("p3 : {}", p3);
	std::println("m2 : {}", m2);

	return 0;
}
