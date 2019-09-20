#include "pch.h"

// HEADERs
#include <iosfwd>  // forward
#include <streambuf>   // basic_streambuf
#include <istream>		// basic_istream
#include <ostream>		// basic_ostream
#include <iostream>		//  // defined cin cout cerr clog
#include <sstream>	// stringstream
#include <fstream>  // filestream
#include <iomanip>  // setbase setfill setw  setprecision


/*

rdbuf  -> raw data buf

Direction:
	input:   stream send data to int
	output:   data out to stream



File; 
	{ofstream, ifstream, fstream}
	std::ios::  
		in			input
		out			output
		app			append      每次输出都在末尾
		ate			at end      to end in openning
		trunc		truncate
		binary		binary

	>>: will skip white space

namesapce std::ios_base

Manipulator:   

   boolalpha / nonboolalpha
   showbase / noshowbase
   showpos / noshowpos    : 3.14 -> +3.14 , show + if >0
   uppercase / nouppercase

   hex/dec/oct
   scientific/fixed

   endl ends flush ws(读入并忽略空格)

   setprecision,
   setw(wide)   // 每次格式化<<, >>这样的IO时都会判断width,如果设置了就按照这个来, 结束前归0,因此每次都需要设置
   setfill()
   left/right (where to fill)

Stream state:   std::ios_base
	goodbit		无错误
	eofbit		结束
	failbit		没有产生期望的结果,  if (!_Changed)  _State |= ios_base::failbit;  看代码, 每次格式化时,都期望Changed改变,如果没变,那肯定是这次操作没成功
	badbit		流不完整

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

TEST(Disabled_Stream, NoEvaled) {
	char buffer[81];
	cin >> setw(81) >> buffer;
	//-
	ostreambuf_iterator<char> it(cout);
	string s{ "hello" };
	copy(s.begin(), s.end(), it);

	//-
	cout << cin.rdbuf();
	//-
	cin >>noskipws>> cout.rdbuf();

}


TEST(Stream, BasicStreamBuf) {  // #include<streambuf>
	// 其构造函数 protected,只能内部使用,, stringbuf and filebuf 由此继承
	//std::streambuf sb;

	//rdbuf -> raw device buf

	/*
		输出肯定默认从尾部开始
		output{
			pbase,  //put base 
			pptr,	//put pointer
			epptr	//end put pointer
		}

		- [pbase(), pptr)  等待flush的数据
		- pptr()== epptr(),  缓冲区满了,调用overflow把对应的数据给送走
		
	*/

	/*
		输入默认从头部开始
		input{
			ebase,	// end back
			egptr   // get pointer
			egptr  // end get pointer
		}
	*/

	// 同时考虑 in out
	// input:  get_area,   从stream取值给data    {eback,gptr,egptr}
	// output:  put_area,  把data放入stream      

	// 里面有不少函数 以pub开头, 意思是public
	//  以s开头:  source(input), sink(output)


	/*
		public:
			sputc, sputn // 放入缓冲区
			sgetc,		 // 返回当前字符,但不consume
			sbumpc,		 // 返回当前c,同时consume	
			sgetn, 
			snextc
			sputbackc,		// 把字符放回缓冲区
			sungetc,		// 回退到前一个字符

		pub* -> 可以让iostream之类的类去使用的,会把请求直接转发给派生类类 
		        pubseekpos  ->  virtual seekpos, 



		派生类需要实现
		virtual :  seekoff, seekpos, setbuf, sync, imbue

		virtual :  _Lock  _Unlock  overflow/underflow  pbackfail showmanyc uflow xsgetn xsputn

		private:  
		//G* for read in   
		//P* for write  out
			_Elem *_Gfirst;	// beginning of read buffer
			_Elem *_Pfirst;	// beginning of write buffer
			_Elem **_IGfirst;	// pointer to beginning of read buffer
			_Elem **_IPfirst;	// pointer to beginning of write buffer
			_Elem *_Gnext;	// current position in read buffer
			_Elem *_Pnext;	// current position in write buffer
			_Elem **_IGnext;	// pointer to current position in read buffer
			_Elem **_IPnext;	// pointer to current position in write buffer

			int _Gcount;	// length of read buffer
			int _Pcount;	// length of write buffer
			int *_IGcount;	// pointer to length of read buffer
			int *_IPcount;	// pointer to length of write buffer
	*/


	// ostream 里面必然会用到streambuf
}

TEST(Stream, BasicStringBuf) {
	std::stringbuf sb;  // inheriting from std::streambuf,也有深远意义， 把字符序列和内存序列关联起来
	   
	// default constructor (mode = in|out)
	std::stringbuf buf1;
	buf1.sputc('1');
	std::cout << &buf1 << '\n';

	// string constructor in at-end mode (C++11)
	std::stringbuf buf2("test", std::ios_base::in
		| std::ios_base::out
		| std::ios_base::ate);
	buf2.sputc('1');
	std::cout << &buf2 << '\n';

	// append mode test (results differ among compilers)
	std::stringbuf buf3("test", std::ios_base::in
		| std::ios_base::out
		| std::ios_base::app);
	buf3.sputc('1');
	buf3.pubseekpos(1);
	buf3.sputc('2');
	std::cout << &buf3 << '\n';

}

TEST(Stream, BasicFileBuf) {
	std::filebuf fb;
}

TEST(Stream, StringStream) {

	// operator >> 代表了格式化， 会有很多限制

	stringstream ss;//basic_stringstream<char, char_traits<char>, allocator<char>>;
	ss << "this is very good" << endl << "hehe" << 12345604 << 12.45 << false << "this is good for that good";
	string a(ss.str());
	cout << a << endl;
	cout << ss.str();


	// 输入是其默认会忽略空格怎么办?
	a.clear();
	
	ss >> a;  // 只能读到 this ,碰到空格自动退出

	/*
	   抽取str时碰到4中情况会自动终止
			1 white space
			2 \0
			3 eof
			4 设置了本次位宽
	*/

	//ss.get()
}

TEST(Stream, IStream) {
	// 输入流必须关联一个streambuf,但可以其不可以直接构造,但是可以使用其派生类比如stringbuf
	stringbuf sb("hello world this is a test for that test",ios::in ||ios::out); // 提供一个字符串来初始化buf, 并且指派buf的模式
	std::istream is(&sb);   // must associate stream with a streambuf,  streambuf means a raw memory  (stringbuf or filebuf)
	
	while (is.good()) {
		string s;
		is >> s; // string 自己定义的 >> operator重载, 
		/*
		  is 虽然是输入流,但是更多情况下其把指责委托给了stringbuf来做通过 is.rdbuf()就额可以获取缓冲区了  
		*/
		cout << s;
	}

	
}


TEST(Stream, IStringStream) {

	std::istringstream input("1\n"
							"some non-numeric input\n"
							"2\n");
	for (;;) {
		int n;
		input >> n;

		if (input.eof() || input.bad()) {
			break;
		}
		else if (input.fail()) {
			input.clear(); // unset failbit
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
		}
		else {
			std::cout << n << '\n';
		}
	}
}




TEST(Stream, FStream) {

	const char* fname = "test_ofstream.txt";

	ofstream ofs(fname, ios::out | ios::trunc);  // will be bound with a basic_filebuf

	ofs << hex << showbase << uppercase << setprecision(10);

	ofs << "some extra code" << "fdsfdssd\a" << endl;

	int i = 0;
	while (i++<100)
	{
		ofs << i << " ";
	}
	ofs.close();

	ifstream ifs(fname);

	ifs.ignore(numeric_limits<streamsize>::max(), '\n');
		
	i = 0;
	while (i++<100 && ifs)
	{
		int k = 0;
		ifs >> hex >> showbase >> uppercase;	 //格式必须一样，否则读不出来
		ifs >> k;
		cout << k << " ";
	}
	

}

TEST(Stream, Redirect) {
	ostringstream ss;
	auto* old = cout.rdbuf();
	cout.set_rdbuf(ss.rdbuf());
	cout << "this is very good" << endl << "hehe" << 12345604 << 12.45 << false;
	cout << "this is very good" << endl << "hehe" << 12345604 << 12.45 << false;
	cout << "this is very good" << endl << "hehe" << 12345604 << 12.45 << false;
	cout << "this is very good" << endl << "hehe" << 12345604 << 12.45 << false;
	cout << flush;
	// restore
	cout.set_rdbuf(old);

	cout << ss.str();

}


class TestManipulator {
public:

	void goodbye() {
		cout << "goodbye" << endl;
	}
	// 允许运算符重载,那么接受一个函数指针也是可以的,给函数指针传入具体的函数名,就可以实现了endl之类的做法
	TestManipulator& operator << (TestManipulator& (*fn)(TestManipulator& th)) {
		fn(*this);
		return *this;
	}
};

TestManipulator& bye(TestManipulator& th) {
	th.goodbye();
	return th;
}

TEST(Stream, SelfManipulator) {
	TestManipulator t;
	t << bye;

}

// using this self-manipulator to skip a line
template<typename _Elem, typename _Traits>
inline std::basic_istream<_Elem, _Traits>& skipln(std::basic_istream<_Elem, _Traits>& is) {
	is.ignore(numeric_limits<streamsize>::max(), is.widen('\n'));
	return is;
}



TEST(Stream, ExplicitException) {
	stringstream ss;

	ss.exceptions(ios::failbit | ios::badbit);

	ss << 12<< "hello world";


	string s;
	long i;
	bool b;
	EXPECT_ANY_THROW( ss >> b);
	//ss >> i;
	//ss >>noskipws>> s;

}
