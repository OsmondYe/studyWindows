#include "pch.h"
#include "helper.hpp"




// keyword:  decltype;
/*
transform(cbegin(aa), cend(aa), std::inserter(keys, keys.begin()), [](decltype(aa)::value_type const& pair) {
	return pair.first;
});

*/
