#include "pch.h"

#include <streambuf>
/*
input/output stream

input:   stream send data to int,double
output:   data out to stream


File; ofstream ifstream  fstream

>>: will skip white space

namesapce std::ios_base

stream : setf unsetf

Manipulator:   wrapper setf的都是永久

   boolalpha / nonboolalpha
   showbase / noshowbase
   showpos / noshowpos    : 3.14 -> +3.14 , show + if >0
   uppercase / nouppercase

   hex/dec/oct
   scientific/fixed

   endl ends flush ws

   setprecision,
   setw(wide) setfill()
   left/right (where to fill)

Stream state:   std::ios_base
	goodbit
	eofbit
	failbit
	badbit

tellg seekg
	
*/

TEST(Stream, Basic) {
	int i = 0, j = 0;
	//cout << "input any 2 ints:";
	//std::cin >> i >> j;
	cout << i << j << endl;

	double d1 = 3.1415926;
	cout << scientific<<setprecision(3) << d1 << setw(10) << setfill('-') << right << d1 << setw(15) << d1 << endl;;
	cout << fixed << setprecision(8) << d1 << setw(10) << setfill('-') << right << d1 << setw(15);
	cout << left << setfill('-') << setw(20) << hex << d1 << endl;
	cout << right << setfill('-') << setw(20) << hex << d1 << endl;


	bool t = true, f = false;
	cout << boolalpha << setw(10) << t << f << endl;
	cout << setw(10) << t << f << endl;
	cout << noboolalpha << setw(10) << t << f << endl;
	cout << setw(10) << t << f << endl;


	i = 0xabc;
	cout << hex << showbase << i << endl;
	cout << hex << noshowbase << i << endl;

}

TEST(Stream, Stringstream) {
	ostringstream ss;//basic_ostringstream<char, char_traits<char>, allocator<char>>;
	ss << "this is very good" << endl << "hehe" << 12345604;
	string aaa(ss.str());
	cout << aaa << endl;
	cout << ss.str();
}