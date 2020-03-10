#include "pch.h"

// range base for
/*
	for( decl: coll){
	}
*/

TEST(Syntax, RangeBaseFor) {

	for (const auto& i : { 1,2,3,4,5,6 }) {
		cout << i << endl;
	}

	// compiler will help 
	auto vec={ 1,2,3,4,5,6 };
	for (auto it = vec.begin(); it != vec.end(); ++it) {
		cout << *it << endl;
	}
	// using iterator-like range, compiler will auto generator code
}