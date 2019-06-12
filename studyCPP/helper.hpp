#include "pch.h"

namespace aux {

	inline static vector<int> getSorted(int size=100) {
		vector<int> rt(size);
		int i = 0;
		std::generate(rt.begin(), rt.end(), [&i]() {return i++; });
		return rt;
	}

	inline static vector<int> getRandom(int size=100) {
		vector<int> rt(size);
		std::generate(rt.begin(), rt.end(), [size]() {return std::rand() % size; });
		return rt;
	}


	template<class Iterator>
	static void output(Iterator beg, Iterator end, const wchar_t* delim =L" ") {
		while (beg != end) {
			wcout << *beg++<<delim;
		}
		wcout << endl;
	}

	template<class Container>
	static void output(Container& c) {
		output(c.begin(), c.end());
	}
}

