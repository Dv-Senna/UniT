#include <print>

#include <UniT/length.hpp>
#include <UniT/time.hpp>
#include <UniT/composed.hpp>

static_assert(UniT::is_same_group_v<
	UniT::UnitGroup<UniT::Second<float>, UniT::Meter<float>, UniT::Kilometer<float>>,
	UniT::UnitGroup<UniT::Second<float>, UniT::Kilometer<float>, UniT::Meter<float>>
>);


int main(int, char**) {

	return 0;
}
