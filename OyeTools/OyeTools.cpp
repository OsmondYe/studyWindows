// OyeTools.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"

#include "cmdline.h"


using namespace std;


string hook_help_message();
void inject_dll(int pid, const string& dll_path);
void inject_dll(const string& pname, const string& dll_path);

inline void lookup_english_word_in_browser(const std::string word) {
	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	std::string url = "http://dict.youdao.com/w/";
	url += word;
	SHELLEXECUTEINFOA data{ 0 };
	data.cbSize = sizeof(SHELLEXECUTEINFOW);
	data.lpFile = url.c_str();
	data.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
	data.nShow = SW_SHOWNORMAL;
	::ShellExecuteExA(&data);
}

cmdline::parser gp;
// parse int got some problem, so only use string
void config_parser(cmdline::parser& gp) {
	gp.add("help", 'h', "show help");
	// for inject dll into target process
	gp.add("inject_dll", 'i', "inject dll into running process, -i --target_id 1024 --dll_path [filePath]");
	gp.add<string>("target_id", '\0', "set target process id", false);
	gp.add<string>("target_name", '\0', "set target process name", false);
	gp.add<string>("dll_path", '\0', "set dll to be injected into target process", false);

	// for english
	gp.add<string>("english_lookup", 'e', "lookup english work in Youdao.com", false);

}


int main(int argc, char* argv[])
{
	config_parser(gp);
	gp.parse_check(argc, argv);

	if (argc == 1 || gp.exist("help")) {
		cout << "==============OYE_TOOLS=================\n";
		cout << gp.usage();
		cout << "========================================\n";
		cout << hook_help_message();
		return 0;
	}

	if (gp.exist("inject_dll")) {
		string dll_path = gp.get<string>("dll_path");
		if (gp.exist("target_id")) {
			string pid = gp.get<string>("target_id");
			inject_dll(std::stoi(pid), dll_path);
		}
		else if (gp.exist("target_name")) {
			string pname = gp.get<string>("target_name");
			inject_dll(pname, dll_path);
		}
		else {
			cerr << "need target_id or target_name\n";
			return 1;
		}		
	}
	else if (gp.exist("english_lookup")) {
		string word = gp.get<string>("english_lookup");
		lookup_english_word_in_browser(word);
	}
	else {
		// by default, failed to parge,	
		cout << "Failed parsing options, Unknown what you want to go\n";
		cout << gp.usage() << endl;
	}
	return 0;
}