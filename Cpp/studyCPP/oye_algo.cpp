#include "pch.h"


TEST(OyeAlgo, Count) {
	static std::map<unsigned char, int> cQuickMap{
		{0b0000, 0 },
		{0b0001, 1 },
		{0b0010, 1 },
		{0b0011, 2 },
		{0b0100, 1 },
		{0b0101, 2 },
		{0b0110, 2 },
		{0b0111, 3 },
		{0b1000, 1 },
		{0b1001, 2 },
		{0b1010, 2 },
		{0b1011, 3 },
		{0b1100, 2 },
		{0b1101, 3 },
		{0b1110, 3 },
		{0b1111, 4 },
	};

	int a = sizeof(int);
	for (int i = 0; i < 100; i++) {
		int t = ::rand();
		cout << showbase << std::hex << t << "\t";
		int c = 0;

		for (int s = 0; s < sizeof(int)-1; s++) {
			unsigned char x = t >> (s*8);
			c += cQuickMap[x&0b00001111];			
			c += cQuickMap[x>>4];			
		}
		cout
			<< std::bitset<sizeof(int) * 8>(t)<<"\t"
			<< std::dec << c << std::endl;

	}
}