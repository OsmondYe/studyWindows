#include "stdafx.h"
#include "utils.h"
//use DISABLED_ prefix to filter out 
using namespace std;


TEST(Utils,sha1){

	std::string str = "The Quick Brown Fox Jumps Over The Lazy Dog,the quick brown fox jumps over the lazy dog";
	std::string exp_rt("fdf652d252642deafaebf1786c17d13d604180aa");

	std::vector<unsigned char> fix_buf;
	const size_t fix_len = 20;
	fix_buf.reserve(fix_len);
		
	win::crypt::sha1((const unsigned char*)str.c_str(), str.length(), fix_buf.data());
	std::string rt = win::str::from_digits(fix_buf.data(), fix_len);
	
	EXPECT_STRCASEEQ(rt.data(), exp_rt.data());

}