#include "pch.h"
#include "helper.hpp"
/*
	keywrod:
		trailing pointer
		walk  traverse
		transplant
		recursion
*/


//recursion:  f(n)=n!  
// 到20!就不行了
unsigned long long factorial(int n) {
	if (n <= 1) {return 1;}			// base component, 递归出口
	return n * factorial(n - 1);	// recursive component 朝着递归出口方向前进
}

unsigned long long factorial_sum(int n) {
	if (n == 0) return 0;
	return n + factorial_sum(n - 1);
}

int gcd(int x, int y) {
	if (y == 0) return x;
	return gcd(y, x % y);
}


TEST(DSBase, basic) {
	// cacl n!
	for (int i = 0; i < 20; i++)
	{
		cout << i << "!=" << factorial(i) << endl;
	}
	for (int i = 0; i < 100; i++)
	{
		cout << "factorial_sum" << i << "=" << factorial_sum(i) << endl;
	}
	
}

// divide and conquer
template<class T>
bool minmax(T a[], int n, int& indexOfMin, int& indexOfMax)
{// Locate min and max elements in a[0:n-1].
 // Return false if less than one element.
	if (n < 1) return false;

	if (n == 1)
	{
		indexOfMin = indexOfMax = 0;
		return true;
	}

	// n > 1
	int s = 1;              // start point for loop
	if (n % 2 == 1)  // n is odd
		indexOfMin = indexOfMax = 0;
	else
	{// n is even, compare first pair
		if (a[0] > a[1])
		{
			indexOfMin = 1;
			indexOfMax = 0;
		}
		else
		{
			indexOfMin = 0;
			indexOfMax = 1;
		}
		s = 2;
	}

	// compare remaining pairs
	for (int i = s; i < n; i += 2)
	{
		// find larger of a[i] and a[i + 1], then compare larger one
		// with a[p.second] and smaller one with a[p.first]
		if (a[i] > a[i + 1])
		{
			if (a[i] > a[indexOfMax])
				indexOfMax = i;
			if (a[i + 1] < a[indexOfMin])
				indexOfMin = i + 1;
		}
		else
		{
			if (a[i + 1] > a[indexOfMax])
				indexOfMax = i + 1;
			if (a[i] < a[indexOfMin])
				indexOfMin = i;
		}
	}

	return true;
}

TEST(DSBase, myMinMax) {
	int a[] = { 3, 2, 4, 1, 6, 9, 8, 7, 5, 0 };
	int n = 10, theMin, theMax;

	// output the array elements
	cout << "a[0:9] = ";
	copy(a, a + n, ostream_iterator<int>(cout, " "));
	cout << endl;

	// locate min and max
	minmax(a, n, theMin, theMax);

	// output locations and values
	cout << "The min element is at " << theMin << endl
		<< "Its value is " << a[theMin] << endl;
	cout << "The max element is at " << theMax << endl
		<< "Its value is " << a[theMax] << endl;
}



template<typename T>
void permutation(T l[], int beg, int end) {
	if(beg+1 ==end){
		std::copy(l, l + end, ostream_iterator<T>(cout, "")); 
		cout << endl;
		return;
	}
	for (int i = beg; i < end; ++i) {
		std::swap(l[beg], l[i]);
		permutation(l, beg+1, end);
		std::swap(l[beg], l[i]);
	}
}

void permutation(string& str, string::iterator beg, string::iterator end) {
	if (beg == end) {
		copy(str.begin(), str.end(), ostream_iterator<char>(cout, ""));
		return;
	}

	for (auto i = beg; i != end; ++i) {
		iter_swap(beg, i);
		permutation(str, beg + 1, end);
		iter_swap(beg, i);
	}
}



TEST(DSBase, Perm) {

	char ss[] ={ "abcde" };
	permutation(ss, 0, 4);
	string s = "abcd";
	permutation(s, s.begin(), s.end());

}

// 不用任何变量,交换2个int的值
void xor_trait() {
	// 异或的特征是什么;
	unsigned int a = rand();
	cout << a << endl;
	unsigned int b = (a ^ a); // 任何数自身异或结果为0
	cout << b << endl;
	b = (a ^ 0);  // 任何数xor 0 没有意义
	cout << b << endl;
	b = (a ^ -1); // 按位和1xor, 结果就是翻转 flip
	cout << b << endl;

}
TEST(DSBase, Bit_Swap) {
	xor_trait();
	int a = rand();
	int b = rand();
	//wcout << L"不用任何变量,交换2个int的值\n";
	cout << "a,b " << a << "  " << b << endl;

	// swap

	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
	cout << "a,b " << a << "  " << b << endl;
}

// 整数二进制中有多少个1
TEST(DSBase, Bit_Count1) {
	auto method1 = [](int i) {
		int r_c = 0;
		int c = 0;
		while (r_c <= 31)
		{
			c += (i >> r_c++) & 1;
		}
		cout <<std::bitset<32>(i)<<"\t"<< c << endl;
	};

	auto method2 = [](int i) {
		int c = 0, t = i;
		while (i)
		{
			i = i & (i - 1); //  i&(i-1) 抹去1最右边的1
			c++;
		}
		cout << std::bitset<32>(t) << "\t" << c << endl;
	};

	for (size_t i = 0; i < 16; i++)
	{
		method1(i);
		method2(i);
	}
}


//一个乱序数组, 所有数都会出现偶数次,只有一个数只有奇数次,找到这个数
TEST(DSBase, Bit_FindOddInEvens) {
	int a[] = { 2,2,4,4,9,9,100,8,8,8,8 };
	int x = 0;
	for (size_t i = 0; i< sizeof(a) / sizeof(int); i++)
	{
		x ^= a[i];
	}
	cout << x << endl;

	//非算法形式虽然结构工整,但是效率不高
	std::map<int, int> m;
	for (size_t i = 0; i< sizeof(a) / sizeof(int); i++) {
		m[a[i]]++;
	}
	// find in map
	auto iter=std::find_if(m.begin(), m.end(), [](std::pair<int,int> i) {
		return i.second == 1;
		}
	);
	if (iter != m.end()) {
		cout << iter->first << endl;
	}

}
//n阶就转为n进制,然后不进位加搞定此类问题
TEST(DSBase, Bit_FindOddInKKK) {
	int a[] = {
		8,8,8,
		2,2,2,
		7,7,7,
		3,
		9,9,9,
		23,23,23,
		67,67,67,
	};

}

// 字符串互为变形词   s1="aapppetc"  s2="eaappptc"
// 对应字符个数一样,但位置不一样
bool is_deformation(const std::string& s1, const std::string& s2) {
	if (s1.length() != s2.length()) {
		return false;
	}
	// preapre a char[255]table;
	int char_count_table[255] = { 0 };

	for (char c : s1) {
		char_count_table[c]++;
	}
	for (char c : s2) {
		char_count_table[c]--;
	}
	return std::all_of(char_count_table, char_count_table + 255, [](int c) {return c == 0; });
}
TEST(DSBase, Str_Deformation) {

	string s1 = "aapppetc", s2 = "eaappptc";

	cout << std::boolalpha << is_deformation(s1, s2) << endl;

	s2 = "cteaappp";

	cout << std::boolalpha << is_deformation(s1, s2) << endl;
	s2 = "teacher";
	cout << std::boolalpha << is_deformation(s1, s2) << endl;


}

// 字符串中数字求和
//	 忽略小数点, 奇数个-,数字为负数,偶数个-,数字为正
int sum_extract_num_in_str(const std::string& s) {
	//典型的遍历过程中记录各种状态的问题
	int sum = 0;
	string parsedNum;
	bool during_parseNum = false;
	bool end_parseNUm = false;		// 经验 end和during状态必然是互斥的,所以要同时被改变,否则一定会有其他问题
	for (char c : s) {
		// check each item, and set each state and value
		if (std::isdigit(c)) {
			during_parseNum = true;
			end_parseNUm = false;
			parsedNum.push_back(c);
			continue;
		}
		else {
			during_parseNum = false;
			end_parseNUm = true;
		}

		if (!parsedNum.empty()&& end_parseNUm) {
			//found
			sum += std::stoi(parsedNum);
			// reset state
			parsedNum.clear();
			during_parseNum = end_parseNUm = false;
			continue;
		}
	}
	// 一个坑, 如果结尾正好是数字,这里需要做一次额外的处理
	if (!parsedNum.empty()) {
		sum += std::stoi(parsedNum);
	}

	return sum;
}

TEST(DSBase, Str_SumNums) {
	string s1 = "1ddd2ccc78f19";
	cout << sum_extract_num_in_str(s1) << endl;
	
	s1 = "100nnn200nnn300xxx400xxx15x";
	cout << sum_extract_num_in_str(s1) << endl;
}

// 去除str中连续出现的k个x的字串
TEST(DSBase, Str_TrimK_Sec_Char) {
	auto trim_k_seq_char = [](string& str, char x, int count) {
		string sub(count, x);
		string::size_type pos = string::npos;
		while (str.npos != (pos = str.find(sub))) {
			str.replace(pos, count, " ");
		}
	};

	string s = "helloaaaaworldaaaachinaaaaahangzhouaaaa";
	trim_k_seq_char(s, 'a', 4);
	cout << s << endl;
}

TEST(DSBase, Str_Replace_all) {
	auto replace_all = [](string& s, const string& from, const string& to) {
		string::size_type pos = string::npos;
		while (s.npos != (pos = s.find(from, 0))) {
			s.replace(pos, from.length(), to);
		}
	};
	
	string s = "helloaaaaworldaaaachinaaaaahangzhouaaaa";
	replace_all(s, "aaaa", "____");
	cout << s << endl;

}

// 互为旋转词
TEST(DSBase, Str_IsRotate) {
	// a,b:  c=a+a, return c.find(b)
	auto is_rotate = [](const string& s1, const string& s2) {
		if (s1.length() != s2.length()) {
			return false;
		}
		string judge = s1 + s1;
		return judge.find(s2) != judge.npos;
	};

	string s1 = "china", s2 = "hinac";
	cout << is_rotate(s1, s2) << endl;
	s2 = "hello";
	cout << is_rotate(s1, s2) << endl;
}
// "aaabbde" ->"a_3_b_2_d_1_e_1
TEST(DSBase, Str_Statistic) {
	auto statistic = [](const string& s) {
		string rt;
		char cur; 
		int count = 0;
		for (char c : s) {
			if (count == 0) {
				//init state
				cur = c;
				count++;
				continue;
			}
			if (cur == c) {
				count++;
				continue;
			}
			else {
				// other 
				rt += (string() + cur + '_' + std::to_string(count) + '_');

				//update new c
				cur = c;
				count = 1;			
			}

		}
		//trim
		if (rt.back() == '_') {
			rt.pop_back();
		}
		return rt;
	};
	string s = "aaaatwwwffdsfsbvbbbbbmmmmpppooowwww";
	cout <<s<<"\n"<< statistic(s) << endl;
}

TEST(DSBase, Str_all_char_uniq) {
	auto is_unique = [](const string& s) {
		std::map<char, int> m;
		for (auto c : s) {
			m[c]++;
		}
		return std::all_of(m.begin(), m.end(), [](std::pair<char, int> iter) {return iter.second == 1;});
	};

	string s = "teacher";

	cout << is_unique(s) << endl;
	cout << is_unique("abcd") << endl;
}



// fibonacci
static int c_fib = 0;
unsigned long long fibonacci(int n) {
	++c_fib;
	if (n == 0) return 0;
	if (n == 1)  return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

TEST(DSBase, Fibonacci) {
	int count = 40;
	/*
		Divide and Conqure
	*/
	for (int i = 3; i < count; i++) {
		int fib_num = i;
		c_fib = 0;
		cout << "fib_num:" << fib_num << "\tresult:" << fibonacci(fib_num) << "\tc_fib:" << c_fib << endl;
	}

	/*
		Dynamic Programming
	*/
	vector<int> fn(count, 0);
	fn[0] = 0;
	fn[1] = 1;
	c_fib = 0;
	for (int i = 2; i < count; i++) {
		fn[i] = fn[i - 1] + fn[i - 2];
		cout << "fib_num:" << i << "\tresult:" << fn[i] << "\tc_fib:" << ++c_fib << endl;
	}

}


/*
	123 -> 321
	-123 -> -321
	120 -> 21;
*/

int reverser_int(int i) {
	//bool f = i & (1 << 31);
	// sanity check
	// return 0, if reversed-int overflow	
	int sum = 0;

	while (i)
	{
		sum *= 10;
		sum += i % 10;
		i /= 10;
	}

	if (sum< numeric_limits<int>::min() || sum> numeric_limits<int>::max()) {
		return 0;
	}

	//return f ? sum * -1: sum;
	return sum;

}

TEST(DSBase, ReverseInt) {

	aux::println(numeric_limits<int>::min());
	aux::println(numeric_limits<int>::max());


	EXPECT_EQ(123, reverser_int(321));
	EXPECT_EQ(-123, reverser_int(-321));
	EXPECT_EQ(21, reverser_int(120));
	cout << reverser_int(1534236469);

}
