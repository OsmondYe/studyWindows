// funcLession.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "funcdetails.h"
/*
	Designed for tech-sharing lesson at 11/16,
	Details behind the virtual function,
	predependent: details behind function calling viewed from asm persective
*/



int main()
{
	//MotherFucker f;
	//Base* p = NULL;  // trigger virtual 
	////////////
	//Base b1;
	//Derived1 d1;
	//Derived2 d2;
	//b1.f1(0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//d1.f1(0xd1d1d1d1, 0xd1d1d1d1, 0xd1d1d1d1, 0xd1d1d1d1);
	////
	////
	////
	//p = &b1;
	//p->f1(0x00000000, 0x00000000, 0x00000000, 0x00000000);
	//p->f3(0x03030303, 0x03030303, 0x03030303, 0x03030303);
	////
	////
	////
	//p = &d1;
	//p->f1(0x00000000, 0x00000000, 0x00000000, 0x00000000);
	//p->f3(0x03030303, 0x03030303, 0x03030303, 0x03030303);
	////
	////
	////
	//p = &d2;
	//d2.f1(0xd2d2d2d2, 0xd2d2d2d2, 0xd2d2d2d2, 0xd2d2d2d2);
	//p->f1(0x00000000, 0x00000000, 0x00000000, 0x00000000);
	//d2.f3(0xd2d2d2d2, 0xd2d2d2d2, 0xd2d2d2d2, 0xd2d2d2d2);
	//p->f3(0x00000000, 0x00000000, 0x00000000, 0x00000000);
	////
	////
	////
	//b1 = d1;
	//b1.f1(0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//p = &b1;
	//p->f1(0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//printf("haha");

	//Base b1, b2, b3, b4;
	//b1.f1(0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//b2.f1(99, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//b3.f1(0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);
	//b4.f1(99, 0xb1b1b1b1, 0xb1b1b1b1, 0xb1b1b1b1);

	//Base::Recall();

	//printf("haha");
	//int wp = sizeof(Weapon); //   a          4
	//int t = sizeof(Tank);		// b         b a  8
	//int w = sizeof(Warship);	// b         b a  8 
	//int a = sizeof(AmphiCar);  // c c2 c3    16+12

	AmphiCar car;

	Weapon* p1 = (Weapon*)(Tank*)&car;
	Weapon* p2 = (Weapon*)(Warship*)&car;

	int c = 0x8888888;
	p1->set();
	c = 0x99999999;
	p2->set();
	bool r = p1 == p2;

	std::cout << r;

}


