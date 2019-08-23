#include "pch.h"
#include "helper.hpp"

using namespace std;
using namespace aux;

/*
 iterartor is a smarte pointer
 [begin,end)
 empty:=  begin==end
 class to provide overloading operator *, ->  ++ -- +n,-n;
 category: 
	input(read only),       x=*iter
	output(write only),    *iter=x
	forward, 
	bidirectional,
	randomaccess
 frame:	
	forword : public input, output
	bidrciton: public forward
	random : publci forward

 insertor wrapper:
	std::inserter,
	std::back_inserter,
	std::front_inserter
*/


