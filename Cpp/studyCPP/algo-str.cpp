#include "pch.h"
// this is string common algo

// my strchr
char* oye_strchr(const char* string, char ch) {

	while ( *string && *string!=ch ){		
		++string;
	}
	if (*string == ch) {
		return (char*)string;
	}
	return (NULL);
}

// my last occurrence of ch in str
char* oye_strrchr(const char* str, char ch) {
	const char* begin = str;
	// move str to end
	while (*str++);

	// search toward front
	while (--str != begin && *str != ch);	

	return (*str) == ch ? (char*)str : NULL;
}
// search str2 in str1
char* oye_strstr(const char* str1, const char* str2) {

	char* cp = (char*)str1;
	char *s1, *s2;			// alarm char* s1,s2  XXXXXX s2 is char , not char*

	if (!*str2) {
		return (char*)str1;
	}

	//scan whole str1
	while (*cp)
	{
		s1 = cp;
		s2 = (char*)str2;

		// each pos in str1, check sutstr , 
		while (*s1 && *s2 && !(*s2 - *s1)) {
			s1++, s2++;
		}

		if (!*s2) {
			return cp;
		}
		
		cp++;
	}

	return NULL;

}

TEST(Str, Count) {
	char str[] = "hello world";

	cout << oye_strchr(str, 'w') << endl;
	cout << oye_strrchr(str, 'w') << endl;
	// SEH exception	
	//cout << oye_strstr(str, "worlddgdf") << endl;
}


/*
leetcode 20, valid parentheses
*/

bool isValid(string s) {
	//sanity check
	if (s.empty()) {
		return true;
	}
	const char* param = "(){}[]";
	// little init,quick test
	if (s.size() == 1) {
		return s.find_first_of(param, 0) == string::npos;
	}
	std::stack<char> assit;
	auto pos = 0;
	for(; pos != string::npos;++pos){
		pos = s.find_first_of(param, pos);
		if (pos == string::npos) {
			break;
		}
		auto c = s[pos];
		// check if match pair
		if (assit.empty()) {
			assit.push(c);
		}
		else {
			char p = assit.top();
			bool match = false;
			switch (p)
			{
			case '(':
				match = c == ')';
				break;
			case '{':
				match = c == '}';
				break;
			case '[':
				match = c == ']';
				break;
			}
			if (match) {
				assit.pop();
			}
			else {
				assit.push(c);
			}
		}

	}
	return assit.empty();
}

TEST(Str, VP) {
	aux::println("valid parenthesis");
	EXPECT_TRUE(isValid(""));
	EXPECT_TRUE(isValid("abdfdsfs"));
	EXPECT_TRUE(isValid("()"));
	EXPECT_TRUE(isValid("()[]{}"));
	EXPECT_TRUE(!isValid("(]"));
	EXPECT_TRUE(!isValid("([)]"));
	EXPECT_TRUE(isValid("{[]}"));
}

/*
leetcode 68, lenght of last word
*/
int lengthOfLastWord(string s) {
	// sanity check
	if (s.empty()) {
		return 0;
	}
	// add extram trim suffix ' ';
	while (s.back() == ' ') {
		s.resize(s.length() - 1);
	}

	if (s.empty()) {
		return 0;
	}
	// check boundary
	if (s.length() == 1) {
		// consider s[0]=' '
		return s[0] != ' '?1:0;
	}
	
	auto pos = s.rfind(' ', string::npos);
	if (pos == string::npos) {
		return s.size();
	}
	if (pos == s.size() - 1) {

	}
	return s.size()-1 - pos;
}
TEST(Str, LOLW) {
	EXPECT_EQ(0, lengthOfLastWord(""));
	EXPECT_EQ(1, lengthOfLastWord("a"));
	EXPECT_EQ(1, lengthOfLastWord(" a"));
	EXPECT_EQ(1, lengthOfLastWord("a "));
	EXPECT_EQ(1, lengthOfLastWord("a b"));
	EXPECT_EQ(1, lengthOfLastWord("a b "));
	EXPECT_EQ(3, lengthOfLastWord("day"));	// only one word can be regard as the last word
	EXPECT_EQ(5, lengthOfLastWord("Hello World"));
}	


/*Longest Substring Without Repeating Characters
Input: "abcabcbb"
Output: 3

Input: "bbbbb"
Output: 1

Input: "pwwkew"
Output: 3
-------------
using dp, exsit optimize value
	-dp[i],  longest must incldue s[i];
	-dp[i] ? dp[i-1]
		- char(dp[i]) never counted,
			dp[i]=dp[i-1]+1;
		- char(dp[i]) has counted in  previous count
			d= dist(i - dp[i]) 


*/

int lswrc(std::string& s) {
	// sanity check
	if (s.empty()) return 0;
	if (s.size() == 1) return 1;
	
	//
	vector<int> assist(256, -1);
	vector<int> dp(s.size(), -1);
	//
	dp[0] = 1;
	assist[s[0]] = 0;

	for (int i = 1; i < s.size(); ++i) {
		if (assist[s[i]] == -1) {
			//s[i] that is never count in
			dp[i] = dp[i - 1] + 1;
		}
		else {
			int dist = i -  assist[ s[i]];
			if (dist > dp[i - 1]) {
				dp[i] = dp[i - 1] + 1;
			}
			else if (dist == dp[i - 1]) {
				dp[i] = dp[i - 1];// like: abcda, 
			}
			else {
				// disp < dp[i-1]
				dp[i] = dist;	// like: abdcd dp[end()-1]=2;  cd
			}
		}
		assist[s[i]] = i;
	}

	// return max(dp);
	return *std::max_element(dp.begin(), dp.end());

}

TEST(Str, LSWRC) {
	cout << "longest substr without repeating chars" << endl;


	string s{ "abc" };
	EXPECT_EQ(3, lswrc(s));

	s = "aaa";
	EXPECT_EQ(1, lswrc(s));

	s = "abcda";
	EXPECT_EQ(4, lswrc(s));

	s = "pwwkew";
	EXPECT_EQ(3, lswrc(s));

	s = "abcabcbb";
	EXPECT_EQ(3, lswrc(s));

	s = "dvdf";
	EXPECT_EQ(3, lswrc(s));

	s = "ohvhjdml";
	EXPECT_EQ(6, lswrc(s));

	int i = 10;
	while (i--)
	{
		auto v = aux::getRandom_LowerAlpha(30);
		aux::output(v);
		aux::println(lswrc(v));
	}

}


/*
longest palindromic substring
like aba, acdca,

table[size,size];  
	- table[i][i]=1 : i in size
	- table[i][i+1]=1  if s[i]==s[i+1]
	- table[i][j]{
		= table[i+1][j-1]  : s[i]==s[j]
		= 0 :  s[i]!=s[j]
	}
*/

std::pair<int, int> get_LPS(const string& str) {
	// sanity check
	if (str.empty()) return make_pair(-1, -1);
	if (str.size() == 1) return make_pair(0, 0);

	vector<vector<int>> table(str.size(),vector<int>(str.size(),0));

	// construct
	for (int delta = 0; delta < str.size(); ++delta) {
		for (int i = 0; i < str.size(); ++i) {
			if (i + delta < str.size()) {
				if (delta == 0) {
					table[i][i] = 1;
				}
				else if (delta == 1) {
					table[i][i + 1] = str[i] == str[i + 1] ? 1 : 0;
				}
				else {
					table[i][i+delta]= str[i] == str[i + delta] ? table[i+1][i+delta-1] : 0;
				}
			}
		}
	}
	// debug
	//for (auto i : table) {
	//	for (auto j : i) {
	//		cout << j<<" ";
	//	}
	//	cout << endl;
	//}
	// how to define maximun value?
	pair<int, int> max_rt{ 0,0 };
	for (int i = 0; i < table.size();++i) {
		auto it = find(table[i].rbegin(), table[i].rend(), 1);
		auto index = table[i].rend() - it-1;
		if ((index - i + 1) > (max_rt.second - max_rt.first + 1)) {
			max_rt.first = i;
			max_rt.second = index;
		}
	}
	// rt
	return max_rt;
}

TEST(Str, LPS) {
	string str = "abcba";
	str = "aaaabbaa";
	auto x = get_LPS(str);
	cout << str.substr(x.first, x.second - x.first + 1);
}