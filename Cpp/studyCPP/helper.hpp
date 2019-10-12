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



namespace expe {

	class ExperimentObj {
	public:
		ExperimentObj() {
			cout << "ExperimentObj contructor" << endl;
		}

		ExperimentObj(ExperimentObj&& other)
			: str_(other.str_),wstr_(other.wstr_)
		{
			cout << "ExperimentObj contructor with move-constructor" << endl;
			// set other 's data member is null
			
		}

		~ExperimentObj() {
			cout << "ExperimentObj destructor" << endl;
		}

		ExperimentObj& operator=(ExperimentObj&& other) {
			return MoveOjb(other);
		}

		ExperimentObj(const ExperimentObj&) = delete;
		ExperimentObj& operator=(const ExperimentObj&) = delete;


	private:
		ExperimentObj& MoveOjb(ExperimentObj& other) {
			this->str_ = other.str_;
			this->wstr_ = other.wstr_;
			return *this;
		}

	private:
		string str_;
		wstring wstr_;
	};


	inline void test_fun(int i) {
		for (size_t i = 0; i < 100; i++)
		{
			cout << i<<" ";
		}
	};

}