// CppTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void binary_to_ascii(unsigned int value);
int main()
{
	/*char aaa[100];

	char bbb[100] = { 0 };

	__int64 ccc[100] = { 0 };

	if (aaa != bbb) {
		printf("hello world");
	}*/


	binary_to_ascii(4267);

    return 0;
}


void binary_to_ascii(unsigned int value) {
	unsigned int quotient;
	quotient = value / 10;
	if (quotient != 0)
	{
		binary_to_ascii(quotient);
	}
	putchar(value % 10 + '0');
}

