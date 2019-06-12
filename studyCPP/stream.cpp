#include "pch.h"

#include <streambuf>
#include <string>
/*
input/output stream
*/

TEST(Stream, Stringstream) {
	ostringstream ss;
	ss << "this is very good" << endl<<"hehe"<<12345604;
	string aaa(ss.str());
	cout<<ss.str();
}