#include "pch.h"


TEST(Idiom, GoodMacro) {
	
	cout << "require drop compile\n";
#ifndef __cplusplus
#error this is good error prompt and tell user
#endif

	cout << "require to load lib, and do not need to setup config pages\n";
#ifndef _ATL_NO_DEFAULT_LIBS
#pragma comment(lib, "kernel32.lib")
#endif
#ifndef _ATL_USE_WINAPI_FAMILY_PHONE_APP
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#endif // _ATL_USE_WINAPI_FAMILY_PHONE_APP
}

TEST(Idiom, TansferOwnership) {
	//Attach(p.Detach());  // Transfer ownership
}


#define MY_DEPRECATED(_Message) __declspec( deprecated(_Message))

MY_DEPRECATED("hello world, to deprecated")
void hehe() {}

TEST(Idiom, Deprecated) {
	//hehe();  // this will make build error C4996
	cout <<"hello world, to deprecated" << endl;

}
