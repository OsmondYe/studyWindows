#include "pch.h"

using namespace aux;



namespace steel_cut{

	int steelcut_top2bottom_n_max(const std::vector<int>& price, int n) {
		if (n == 0) {
			return 0;
		}
		int q = -1;
		for (int i = 1; i <= n; ++i) {
			q = std::max<int>(q, price[i] + steelcut_top2bottom_n_max(price, n - i));
		};
		return q;
	}



	int steel_cut_memoized(const std::vector<int>& price, int n, std::vector<int>& assit) {
		if (n == 0) {
			return 0;
		}
		if (assit[n] > -1) {
			return assit[n];
		}

		int q = -1;

		for (int i = 1; i <= n; ++i) {
			q = max(q, price[i] + steel_cut_memoized(price, n - i, assit));
		}
		assit[n] = q;
		return q;
	}

	int steel_cut_memoized_n_max(const std::vector<int>& price, int n) {

		vector<int> assit(price.size(), -1);
		assit[0] = 0;
		return steel_cut_memoized(price, n, assit);

	}


	int steel_cut_bottom_up_n_max(const std::vector<int>& price, int n) {
		vector<int> assit(price.size(), 0);

		for (int i = 1; i <= n; ++i) {
			int q = -1;
			for (int j = 1; j <= i; ++j) {
				q = max(q, price[j] + assit[i - j]);
			}
			assit[i] = q;
		}
		return assit[n];
	}


}

TEST(AlgoDP, InToAlgo_DP) {
	using namespace steel_cut;
	println("introduciton to algorithme DP,  steel_cut");
	vector<int> len_price{
		//  0,1,2,3,4,5 ,6 ,7 ,8, 9 10
			0,1,5,8,9,10,17,17,20,24,30
	};
	println("org: length and price\n");
	aux::output(len_price);

	//  iter max(i)
	for (int i = 1; i <= 10; ++i) {
		cout << steelcut_top2bottom_n_max(len_price, i) << "\t"
			<< steel_cut_memoized_n_max(len_price, i) << "\t"
			<< steel_cut_bottom_up_n_max(len_price, i)
			<< endl;
	}
}


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

TEST(AlgoDP, maxSubArray) {
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

TEST(AlgoDP, climbStairs) {
	aux::println("dp, climbing stairs");
	EXPECT_EQ(3, dp_climbStairs(3));
}

/*
leetcoce 121, best time buy and sell
- same as maxinum sub array
Input: [7,1,5,3,6,4]
Output: 5
Input: [7,6,4,3,1]
Output: 0
*/
int maxProfit(vector<int> prices) {
	//sanity check
	if (prices.empty()) {
		return 0;
	}
	if (prices.size() == 1) {
		// exception?
		return 0;
	}
	// for size >=2,  calc differ
	vector<int> differ;
	std::transform(prices.begin(), prices.end() - 1,
		prices.begin() + 1,
		back_inserter(differ),
		[](int l, int r) {return r - l; });

	/* dp: dp[i] , manxisum values that must include i
	   dp[i] = {
			dp[i-1]+s[i]  dp[i-1]>=0;
			s[i];
	   }
	 */
	
	vector<int> dp(differ.size(), 0);
	dp[0] = differ[0];
	for (int i = 1; i < dp.size(); ++i) {
		dp[i] = dp[i - 1] >= 0 ? dp[i - 1] + differ[i] : differ[i];
	}
	// if all(dp)<0, return 0;
	auto i= *max_element(dp.begin(), dp.end());
	return i > 0 ? i : 0;
}

TEST(AlgoDP, BTSS) {
	aux::println("best time buy and sell");
	vector<int> s = { 7,1,5,3,6,4 };
	EXPECT_EQ(5, maxProfit(s));
	EXPECT_EQ(0, maxProfit({ 7,6,4,3,1 }));
}


/*
leetcode 198, house rob
*/
int rob(vector<int> nums) {
	// sanity check 
	if (nums.empty()) {
		return 0;
	}
	if (nums.size() == 1) {
		return nums.front();
	}
	if (nums.size() == 2) {
		return max(nums[0], nums[1]);
	}
	int odd_sum = nums[1];
	int even_sum = nums[0];
	for (int i = 2; i < nums.size(); ++i)
	{
		if (i % 2) {
			odd_sum += nums[i];
		}
		else {
			even_sum += nums[i];
		}
	}
	return max(odd_sum, even_sum);
	// odd_sum and even_sum is not the answer for [2,1,1,2]
	// [a,b,c,d] -> max may is [a,d]
}

int rob_dp(vector<int> nums) {
	// sanity check 
	if (nums.empty()) {
		return 0;
	}
	if (nums.size() == 1) {
		return nums.front();
	}
	if (nums.size() == 2) {
		return max(nums[0], nums[1]);
	}
	if (nums.size() == 3) {
		return max(nums[1], nums[0] + nums[2]);
	}
	/*
	dp[i]= max( dp[i-2]+s[i], dp[i-3]+s[i] );
	*/
	vector<int> dp(nums.size(), 0);
	dp[0] = nums[0];
	dp[1] = nums[1];
	dp[2] = max(nums[1], nums[0] + nums[2]);
	for (int i = 3; i < nums.size(); ++i)
	{
		dp[i] = max(dp[i - 2] + nums[i], dp[i - 3] + nums[i]);
	}
	return *max_element(dp.begin(), dp.end());
}

TEST(AlgoDP, HR) {
	aux::println("dp house rob");
	EXPECT_EQ(4, rob({ 1,2,3,1 }));
	EXPECT_EQ(12, rob({ 2,7,9,3,1 }));
	EXPECT_NE(4, rob({ 2,1,1,2}));


	EXPECT_EQ(4, rob_dp({ 1,2,3,1 }));
	EXPECT_EQ(12, rob_dp({ 2,7,9,3,1 }));
	EXPECT_EQ(4, rob_dp({ 2,1,1,2 }));

}

/*
leetcode 303, range sum query

build dp[i][j] for all possible sub range, upper triangular
delta filling data;

dp[i][j]=s[j]+dp[i][j-1]

*/
int dp_rsq(vector<int> nums,int i, int j) {
	int sum = 0;
	while (i <= j) {
		sum += nums[i++];
	}
	return sum;
}
TEST(AlgoDP, rsq) {
	aux::println("303 dp range sum query");
	vector<int> nums{ -2, 0, 3, -5, 2, -1 };

	// Approach 1, bruthe force
	EXPECT_EQ(1, dp_rsq(nums, 0, 2));
	EXPECT_EQ(-1, dp_rsq(nums, 2, 5));
	EXPECT_EQ(-3, dp_rsq(nums, 0, 5));

	// Approach 2 dp with table 
	vector< vector<int>> dp(nums.size(), vector<int>(nums.size(), 0));
	for (int i = 0; i < nums.size(); ++i) {
		dp[i][i] = nums[i];
	}

	// begin delta increase with upper-triangular fill
	for (int i = 0; i < nums.size(); ++i) {
		for (int j = i+1; j < nums.size(); ++j) {
			dp[i][j] = dp[i][j - 1] + nums[j];
		}
	}

	// this dp is not good ,too slow,
	EXPECT_EQ(1, dp[0][2]);
	EXPECT_EQ(-1, dp[2][5]);
	EXPECT_EQ(-3, dp[0][5]);


	// Approach 3, Mathematical thought,
	// what sum(i,j) mean?  sum(0,j)-sum(0,i-1)?
	vector<int> dp2(nums.size(), 0);
	dp2[0] = nums[0];
	for (int i = 1; i < nums.size(); ++i) {
		dp2[i] = nums[i] + dp2[i - 1];
	}

	EXPECT_EQ(1, dp2[2]);
	EXPECT_EQ(-1, dp2[5]-dp2[1]);
	EXPECT_EQ(-3, dp2[5]);
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
bool is_subsequence(string s, string t) {
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


vector<vector<int>> Pascal_Triangle_Generate(int numRows) {
	vector<vector<int>> rt;
	if (numRows >0) {
		rt.push_back({ 1 });
	}
	if (numRows >1) {
		rt.push_back({ 1,1 });
	}
	if (numRows > 2) {
		rt.push_back({ 1,2,1 });
	}
	for (int i = 3; i < numRows; ++i) {
		vector<int>& pre = rt[i - 1];
		vector<int> cur;
		cur.push_back(1);

		for (int j = 0; j < pre.size()-1; ++j) {
			cur.push_back(pre[j] + pre[j + 1]);
		}

		cur.push_back(1);
		rt.push_back(cur);
	}
	return rt;
}


vector<int> Pascal_Triangle_getRow(int rowIndex) {

	// TBD

	if (rowIndex < 0) {
		return vector<int>();
	}
	vector<int> rt(rowIndex + 1, 0);
	rt[0]=1;
	for (int i = 1; i <= rowIndex; ++i) {

	}
	return rt;

}

TEST(AlgoDP, LeetCode) {
	aux::println("392, is subsequence");
	EXPECT_TRUE(is_subsequence("abc", "ahbgdc"));
	EXPECT_TRUE(is_subsequence("", "ahbgdc"));
	EXPECT_TRUE(is_subsequence("leetcode", "yylyyeyyeyyeyytyycyyoyydyye"));
	EXPECT_FALSE(is_subsequence("axc", "ahbgdc"));

	aux::println("746, min cost climbing stairs");
	vector<int> cost{ 0,0,1,1 };
	EXPECT_EQ(1, dp_mccs(cost));

	cost = { 1,1,0,0 };

	EXPECT_EQ(1, dp_mccs(cost));

	{
		auto x = Pascal_Triangle_Generate(20);

		for (auto i : x) {
			for (auto j : i) {
				cout << j << "  ";
			}
			cout << endl;
		}

		auto y = Pascal_Triangle_getRow(3);
	}
}