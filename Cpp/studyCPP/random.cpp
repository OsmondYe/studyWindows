#include "pch.h"
#include "helper.hpp"


#include <random>

/* 
	c11提供了多种不同特设的随机数,分布问题.random number and distribution

	分布:
		 均匀(高斯)分布	uniform
		 伯努利分布		bernoulli
		 泊松分布		poisson
		 正太分布		normal
		 抽样分布		discrete


	基本引擎:
		linear_congruential_engine
		mersenne_twister_engine
		substrict_with_carry_engine

*/

TEST(Random, Basic) {
	// 缺省引擎
	std::default_random_engine rge;
	// 
	std::uniform_int_distribution<int> di(0, 100);
	for (int i = 0; i < 100; i++) {
		cout << di(rge) << " ";
	}
	cout << endl;

	
	for (unsigned char c = numeric_limits<unsigned char>::min(); c < numeric_limits<unsigned char>::max(); ++c) {
			cout <<left<<hex<<int(c)<<":" <<c << " ";
		if (0 == c % 20) {
			cout << endl;
		}
	}

	cout << endl;
	for (size_t i = 0; i < 100; i++)
	{
		cout << char(0xdb);
	}

	cout << endl;
	for (size_t i = 0; i < 100; i++)
	{
		cout << char(0xdc);
	}

	cout << endl;
	for (size_t i = 0; i < 100; i++)
	{
		cout << char(0xdd);
	}

	cout << endl;
	for (size_t i = 0; i < 100; i++)
	{
		cout << char(0xde);
	}

	cout << endl;
	for (size_t i = 0; i < 100; i++)
	{
		cout << char(0xdf);
	}
}

TEST(Random, Shuffle) {
	auto v = aux::getSorted(10);
	aux::output(v);
	std::default_random_engine dre;
	//1
	std::shuffle(v.begin(), v.end(), dre);
	aux::output(v);
	//2
	std::shuffle(v.begin(), v.end(), dre);
	aux::output(v);
	//3
	std::shuffle(v.begin(), v.end(), dre);
	aux::output(v);

}

//使用图形化进行计数
TEST(Random, GraphicPrint) {
	std::map<int, int> i2count;
	std::default_random_engine rge;
	// [1,100]
	std::uniform_int_distribution<int> di(1, 10);
	for (int i = 0; i < 4000; i++) {
		auto r = di(rge);
		i2count[r] = ++i2count[r];
	}

	cout << endl;
	// 均匀分布,每个数都差多
	cout << "Uniform Distribution:" << endl;
	for (auto p : i2count) {
		cout <<right<<setw(2)<< p.first << " ";
		for (int i = 0; i < p.second / 10; ++i) {
			cout << char(0xdb);
		}
		cout << endl;
	}


	i2count.clear();
	cout << "Normal Distribution:" << endl;
	std::normal_distribution<float> ndi(1, 10);
	for (int i = 0; i < 4000; i++) {
		int r = (int)ndi(rge);
		i2count[r] = ++i2count[r];
	}
}


TEST(Random, SeedSeq) {
	// 提供一些足够随机的种子
	std::seed_seq ss{ 1,2,3,4,5};
	std::vector<unsigned int> v(10);
	ss.generate(v.begin(), v.end());

	aux::output(v);
}

TEST(Random, GeneratorCanonical) {
	// 提供随机浮点数 [0,1)	
	std::random_device rd;
	std::default_random_engine	dre(rd());
	for (size_t i = 0; i < 10; i++)
	{
		cout << std::generate_canonical<float, 2>(dre) << " ";
	}
	cout << endl;
}

TEST(Random, RandomDevice) {
	std::random_device rd;
	for (size_t i = 0; i < 100; i++)
	{
		cout << rd() << " ";
	}
	cout << endl;
}

TEST(Random, Engine) {
	// 引擎用来提供生成的数据
	std::linear_congruential_engine< std::uint_fast32_t, 16807, 0, 2147483647 > lce;
	std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31,
		0x9908b0df, 11,
		0xffffffff, 7,
		0x9d2c5680, 15,
		0xefc60000, 18, 1812433253> mte;
	std::default_random_engine dre;
	
	for (size_t i = 0; i < 100; i++)
	{
		cout << lce() << " " << mte() << " " << dre() << endl;

	}
}

TEST(Random, Distribution) {
	// 分布决定以何种手法讲引擎内的值统一起来
	std::default_random_engine dre;
	std::uniform_int_distribution<int> uid(0,200);
	for (size_t i = 0; i < 100; i++)
	{
		cout << uid(dre) << " ";
	}
	cout << endl;
}