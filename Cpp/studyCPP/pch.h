// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

// c++
#include <iosfwd>
#include <iostream>
#include <iomanip>
#include <cctype>  // isalnum, isalpha, tolower, toupper
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <forward_list>
#include <array>	// psedu-container
#include <queue>
#include <stack>
#include <queue>
#include <iterator>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <random>
#include <memory>	// shared_ptr  unique_ptr, weak_ptr
// c++ excption
#include <exception>
#include <stdexcept>
#include <system_error>
// end c++ excpetion

#include <chrono>
// thread
#include <thread>
#include <mutex>

using namespace std;

// gtest
#include <gtest/gtest.h>



// helper
#include "helper.hpp"

#endif //PCH_H
