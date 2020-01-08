#include "pch.h"
#include "helper.hpp"



/*
leet code 1207
*/
bool uniqueOccurrences(vector<int> arr) {
    map<int, int> cache;
    for (auto i : arr) {
        cache[i]++;
    }
    if (cache.empty()) {
        return true;
    }
    vector<int> counts;

    for (auto i : cache) {
        counts.push_back(i.second);
    }
    std::sort(counts.begin(), counts.end());
    return std::adjacent_find(counts.begin(), counts.end()) == counts.end();

}
TEST(AlgoHash, UNOO) {
    aux::println("Unique Number of Occurrences");
    EXPECT_TRUE(uniqueOccurrences({ 1,2,2,1,1,3 }));
    EXPECT_TRUE(!uniqueOccurrences({ 1,2 }));
    EXPECT_TRUE(uniqueOccurrences({ -3,0,1,-3,1,1,1,-3,10,0 }));
}