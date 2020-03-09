#include "pch.h"

/*
- c11 extent the delete keyword
*/

bool isLucky(int x) { return true; }
// I dont want implicit conversion
bool isLucky(char x) = delete;
bool isLucky(double x) = delete;
bool isLucky(bool x) = delete;

TEST(Syntax, DeleteEliminateImplicitConversion) {
	// isLucky(true);   // error
	cout << "error isLucky(true)" << endl;
}


// prevent template instantiation
template<typename T> void processPointer(T* prt) {}

// I dont like void* and char* in processPointer
template<> void processPointer(void* prt) = delete;
template<> void processPointer(char* prt) = delete;

TEST(Syntax, PreventTemplateInstantiation) {
	cout << "for template<typename T> void processPointer(T* prt) {} \n";
	cout << "I dont like void* and char* in processPointer" << endl;
}