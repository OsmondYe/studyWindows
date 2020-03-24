#include "pch.h"

// HEADERs
#include <regex>


/*
Match		regex_match
Search		regex_search
Tokenize
Replace
*/

/*

.			除\n外的任何字符,
. | \n      所有字符 . 或者 \n
*			前面的模式重复0次或多次

*/


TEST(Regex, Basic) {

}

TEST(Regex, Match) {

	auto rm = [](const string& str, const string& pattern) {
		cout << endl;
		cout << "str:\t" << str << endl;
		cout << "pattern:\t" << pattern << endl;
		cout << "result:\t" << boolalpha << regex_match(str, regex(pattern)) << endl;
		cout << endl;
	};

	string xmlValue{ "<tag>values</tag>" };
	string xmlValue2{ "<tag>values</no_tag>" };

	string str_pattern{ "<.*>.*</.*>" };

	// 可以找到,但是有问题,<tag>fsd</fsd> 这样的也被匹配了
	rm(xmlValue, str_pattern);
	rm(xmlValue2, str_pattern);

	// 找不到
	rm(xmlValue, { "<*>*</*>" });


	string str_pattern3{ "<(.*)>.*</\\1>" };  // (.*) ->  capture grouping, \1 表示和第一个匹配组相同
	rm(xmlValue, str_pattern3);   // 精确匹配
	rm(xmlValue, R"(<(.*)>.*</\1>)");   // 精确匹配
	rm(xmlValue2, str_pattern3);  // 前后tag不一致,无法匹配

	string xmlValue3{ "some elements in xml : <tag>values</tag>" };
	rm(xmlValue3, str_pattern3);
	rm(xmlValue3, R"(.*<(.*)>.*</\1>)");  // 首多一个.*


}



TEST(Regex, Search) {
	auto rs = [](const string& str, const string& pattern) {
		cout << endl;
		cout << "str:\t" << str << endl;
		cout << "pattern:\t" << pattern << endl;
		cout << "result:\t" << boolalpha << regex_search(str, regex(pattern)) << endl;
		cout << endl;
	};


	string value1{ "some elements in xml : <tag>values</tag>" };

	rs(value1, R"(<(.*)>.*</\1>)");  // 寻找这样的模式
	rs(value1, R"(.*<(.*)>.*</\1>)");  

	// rs(data,regex(pattern)) == rm(data, regex("(.|\n)*"+pattern+"(.|\n)*")

}

TEST(Regex, Subexpression) {
	string data{ R"(XML tags: LLL<begin_tag_name>the value</for_the_end_tag>LL.the others;)" };
	string pattern{ R"(<(.*)>(.*)</(.*)>)" };
	match_results<string::const_iterator> m;

	bool found = regex_search(data, m, regex(pattern));

	cout << "data:\t" << data<<endl;
	cout << "pattern:\t" << pattern << endl;
	cout<< "m.size():\t" << m.size() << endl;  //number of capture groups
	cout << "m.str():\t" << m.str() << endl;
	cout << "m.length():\t" << m.length() << endl;
	cout << "m.position():\t" << m.position() << endl;
	cout << "m.prefix():\t" << m.prefix().str() << endl;
	cout << "m.suffix():\t" << m.suffix().str() << endl;

	// m中每个组件到底是什么
	/*  m[0] : <begin_tag_name>the value</for_the_end_tag>   用0代表整体
	    m[1] : begin_tag_name
		m[2] : the value
		m[3] : for_the_end_tag
	*/
	for (int i = 0; i < m.size(); ++i) {
		cout << i << "\t" << m.str(i) << endl;
		cout << i << "\t" << m[i].str() << endl;
		cout << i << "\t" << m.position(i) << endl;
		cout << i << "\t" << m.length(i) << endl;
	} 

	// using itor
	for (auto pos = m.begin(); pos != m.end();++pos) {
		//cout << it->;
		cout << *pos << pos->length() << endl;;
	}


}


TEST(Regex, PatternForAll) {
	string data{ R"(
		<person>
			<first>Osmond</first>
			<last>Ye</last>
		</person>
	)" 
	};

	regex reg(R"(<(.*)>.*</\1>)");  // 内容不能有换行

	auto pos = data.cbegin();
	auto end = data.cend();

	smatch m;
	for (; regex_search(pos, end, m, reg); pos=m.suffix().first ) {
		cout << "match:\t" << m.str() << endl;
		cout << "tag:\t" << m.str(1) << endl;
		cout << "value:\t" << m.str(2) << endl;
	}


}

TEST(Regex, Replace) {


}


namespace {

	auto getReplacemap() {
		std::map<wstring, wstring> _tokens;
		const static wchar_t* USER = L"$(User)";
		const static wchar_t* EMAIL = L"$(Email)";
		const static wchar_t* HOST = L"$(Host)";
		const static wchar_t* IP = L"$(IP)";
		const static wchar_t* BREAK = L"$(Break)";  //  to \n
		const static wchar_t* DATE = L"$(DATE)";    //  "YYYY-MM-DD"
		const static wchar_t* TIME = L"$(TIME)";	//  "HH:mm:ss"	

		_tokens[USER] = L"osmond";
		_tokens[EMAIL] = L"osmond.ye@nextlabs.com";
		_tokens[HOST] = L"my working computer";
		_tokens[IP] = L"127.0.0.1";
		_tokens[BREAK] = L"\n";
		_tokens[DATE] = L"2020-03-24";
		_tokens[TIME] = L"14:13:00";

		return _tokens;
	}

	auto escapeSpecial(const wstring& key) {
		wstring rt;
		for (auto i : key) {
			if (i == '$' || i == '(' || i == ')') {
				rt.push_back('\\');
			}
			rt.push_back(i);
		}
		return rt;
	}

}

TEST(Regex, NextlabsWatermarkReplace) {
	wstring  watermark = L"$(User)$(Break)$(Date)$(Break)\n$(User)$(email)\n$(email)\n$(email)\n$(HOST)\n$(IP)$(Break)$(Time)";
	//cout << watermark;
	wstring rt = watermark;
	auto map = getReplacemap();

	for (auto& i : map) {
		const auto flag = std::regex::flag_type::ECMAScript | std::regex::flag_type::icase | std::regex::flag_type::nosubs;

		wregex reg(escapeSpecial(i.first), flag);

		rt = regex_replace(rt, reg, i.second);

	}


	wcout << rt << endl;


}