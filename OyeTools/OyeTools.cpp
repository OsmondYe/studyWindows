/*
 Oye tools,
	- contain Official stable handy helper functions
	- direct command lint support
*/
#include "pch.h"
#include "cmdline.h"




const int version = 0x0001'0102;
void print_version() {
	char buf[255] = { 0 };
	sprintf_s(buf, "Version %d.%02d.%04d Built on %s", 1, 0, 0102, "2020-09-04");
	cout << buf << endl;
}



string hook_help_message();
cmdline::parser gp;


// parse int got some problem, so only use string
void config_parser(cmdline::parser& gp) {
	print_version();


	gp.add("help", 'h', "show help");
	
	// for inject dll into target process
	gp.add("inject_dll", 'i', "inject dll into running process, -i --target_id 1024 --dll_path [filePath]");
	gp.add<string>("target_id", '\0', "set target process id", false);
	gp.add<string>("target_name", '\0', "set target process name", false);
	gp.add<string>("dll_path", '\0', "set dll to be injected into target process", false);
	
	// for shell api english
	gp.add<string>("english_lookup", 'e', "lookup english work in Youdao.com", false);
	gp.add("shell_api_test", '\0', "run shell api test");
	
	// for network
	gp.add("hostname", 'n', "lookup hostname");
	gp.add<string>("ipbyname", 'p', "lookup hostname", false);

	// for gdiplus 
	// to perform picture operation
	gp.add("pic_grary", 'g', "make as picture gray, --src [src] --dst [out]");
	gp.add<string>("src", '\0', "set source path",false);
	gp.add<string>("dst", '\0', "set output path", false);
	gp.add<int>("pic_op", '\0', "pic operation, 0,1,2,3,4,5,6,7,8,9",false);
	gp.add<int>("pic_op_p_a", '\0', "pic operation param A,int", false);
	gp.add<float>("pic_op_p_b", '\0', "pic operation param B,int", false);

	// for ocr
	gp.add("ocr", 'o', "optical char regocgnize -- src [src]");

	

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
	else if (gp.exist("hostname")) {
		query_host_name();
	}
	else if (gp.exist("ipbyname")) {
		string word = gp.get<string>("ipbyname");
		query_address_by_name(word);
	}
	else if (gp.exist("shell_api_test")) {
		shell_api_test();
	}
	else if (gp.exist("pic_grary")) {
		string src = gp.get<string>("src");
		string dst;
		if (gp.exist("dst")) {
			dst = gp.get<string>("dst");
		}
		int pic_op = 0;
		if (gp.exist("pic_op")) {
			pic_op = gp.get<int>("pic_op");
		}

		int pic_param_a = 0.0;
		if (gp.exist("pic_op_p_a")) {
			pic_param_a = gp.get<int>("pic_op_p_a");
		}

		float pic_param_b = 0.0;
		if (gp.exist("pic_op_p_b")) {
			pic_param_b = gp.get<float>("pic_op_p_b");
		}
		process_picture_gray(src, dst, pic_op, pic_param_a, pic_param_b);
	}
	else if (gp.exist("ocr")) {
		string src = gp.get<string>("src");
		string out;
		process_ocr(src, out);
		cout << out;
	}
	else {
		// by default, failed to parge,	
		cout << "Failed parsing options, Unknown what you want to go\n";
		cout << gp.usage() << endl;
	}
	return 0;
}