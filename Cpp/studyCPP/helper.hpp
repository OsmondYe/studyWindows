#pragma once
#include "pch.h"

namespace aux {

	inline static vector<int> getSorted(int size=100) {
		vector<int> rt(size);
		int i = 0;
		//std::generate(rt.begin(), rt.end(), [&i]() {return i++; });
		iota(rt.begin(), rt.end(), 0);
		return rt;
	}

	inline static vector<int> getRandom(int size=100) {
		vector<int> rt(size);
		std::generate(rt.begin(), rt.end(), [size]() {return std::rand() % size; });
		return rt;
	}

	inline static vector<int> getRandom_UnitForm(int size, int min, int max) {
		std::default_random_engine rge(std::time(NULL));
		std::uniform_int_distribution<int> di(min, max);
		vector<int> rt(size);
		std::generate(rt.begin(), rt.end(), [&rge,&di]() {return di(rge); });
		return rt;
	}

	inline static string getRandom_LowerAlpha(int size) {
		std::default_random_engine rge(std::clock());
		std::uniform_int_distribution<int> di('a', 'z');
		string rt(size,0);
		std::generate(rt.begin(), rt.end(), [&rge, &di]() {return di(rge); });
		return rt;
	}

	template<typename Container>
	static void output(const Container& c) {
		using std::cbegin;
		using std::cend;
		for (auto it = cbegin(c); it != cend(c); ++it) {
			cout << *it << " ";
		}
		cout << endl;
	}

	template<typename T>
	static inline void println(const T& c) {
		std::cout << c << std::endl;
	}

	template<class Container>
	static void shuffle(Container& c) {
		std::mt19937 engine(rand());
		std::shuffle(c.begin(), c.end(), engine);
	}
}