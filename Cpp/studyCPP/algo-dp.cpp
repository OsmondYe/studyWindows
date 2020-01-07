#include "pch.h"
#include "helper.hpp"

/*
leetcode 53  maximum subarry:
Input: [-2,1,-3,4,-1,2,1,-5,4],
Output: 6
Explanation: [4,-1,2,1] has the largest sum = 6.

think:
	dp[i] : sum that must include i
	dp[i] = {
		-  dp[i-1] +s[i]  : dp[i-1]>=0 
		-  s[i]
	}

*/
int dp_maxSubArray(vector<int>& nums) {
	aux::println("dp, max subarry");
	vector<int> dp(nums.size(), -1);
	dp[0] = nums[0];
	for (int i = 1; i < nums.size(); ++i) {
		if (dp[i - 1] >= 0) {
			dp[i] = dp[i - 1] + nums[i];
		}
		else {
			dp[i] = nums[i];
		}
	}
	return *max_element(dp.begin(), dp.end());
}

TEST(DP, MS) {
	vector<int> nums{ -2,1,-3,4,-1,2,1,-5,4 };

	EXPECT_EQ(6, dp_maxSubArray(nums));
}


/*
leetcode 70, climbing stairs
like fabonacci  f(n)= f(n-2) + f(n-1);
*/

int dp_climbStairs(int n) {
	if (n <= 0) {
		return 0;
	}
	if (n == 1) {
		return 1;
	}
	if (n == 2) {
		return 2;
	}

	vector<int> dp(n+1, 0);
	dp[0] = 0;
	dp[1] = 1;
	dp[2] = 2;
	for (int i = 3; i < n+1; ++i) {
		dp[i] = dp[i - 1] + dp[i - 2];
	}
	return dp.back();
}

TEST(DP, CS) {
	aux::println("dp, climbing stairs");
	EXPECT_EQ(3, dp_climbStairs(3));
}

/*
leetcode 303, range sum query
*/
int dp_rsq(vector<int> nums,int i, int j) {
	int sum = 0;
	while (i <= j) {
		sum += nums[i++];
	}
	return sum;
}
TEST(DP, rsq) {
	vector<int> nums{ -2, 0, 3, -5, 2, -1 };

	vector< vector<int>> dp(nums.size(), vector<int>(nums.size(), 0));

	// begin delta increase with upper-triangular fill
	for (int delta = 0; delta < nums.size(); ++delta) {
		for (int i = delta; i < nums.size(); ++i) {

		}
	}


	EXPECT_EQ(1, dp_rsq(nums,0,2));
	EXPECT_EQ(-1, dp_rsq(nums,2,5));
	EXPECT_EQ(-3, dp_rsq(nums,0,5));
}


/*
leetcode 392,  Is subsequence
Example 1:
s = "abc", t = "ahbgdc"
Return true.

Example 2:
s = "axc", t = "ahbgdc"
Return false.
*/
bool is_IS_1(string s, string t) {
	// sanity check
	if (s.empty() && t.empty()) {
		return true;
	}
	if (s.empty() && !t.empty()) {
		return true;
	}
	if (!s.empty() && t.empty()) {
		return false;
	}
	size_t pos_t = 0;
	for (auto c : s) {
		pos_t = t.find(c, pos_t);
		if (pos_t == string::npos) {
			return false;
		}
		pos_t++;
	}
	return true;

}
TEST(DP, IS) {
	aux::println("is subsequence");
	EXPECT_TRUE(is_IS_1("abc", "ahbgdc"));
	EXPECT_TRUE(is_IS_1("", "ahbgdc"));
	EXPECT_TRUE(is_IS_1("leetcode", "yylyyeyyeyyeyytyycyyoyydyye"));
	EXPECT_FALSE(is_IS_1("axc","ahbgdc"));
}

/*
leetcode 746. Min Cost Climbing Stairs
pay cost[i] move 1 or 2 steps,
reach top, using min cost,
start on cost[0] or cost[1]

i>=3
dp[i]=c[i]+min(dp[i-1]+dp[i-2])

return min(dp[n-1],dp[n-2]);


*/
int dp_mccs(vector<int>& cost) {
	if (cost.empty()) {
		return 0;
	}
	if (cost.size() == 1) {
		return cost[0];
	}
	if (cost.size() == 2) {
		return std::min(cost[0], cost[1]);
	}
	vector<int> dp(cost.size(), 0);
	dp[0] = cost[0];
	dp[1] = cost[1];
	for (int i = 2; i < cost.size(); ++i) {
		dp[i] = cost[i] + std::min(dp[i - 1], dp[i - 2]);
	}
		
	return std::min(dp[dp.size() - 1], dp[dp.size() - 2]);

}
TEST(DP, mccs) {
	aux::println("min cost climbing stairs");
	vector<int> cost{ 0,0,1,1 };
	EXPECT_EQ(1, dp_mccs(cost));

	cost = { 1,1,0,0 };
	EXPECT_EQ(1, dp_mccs(cost));


}