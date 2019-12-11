#include "pch.h"


// in include <>(predefine list) and ""(cur dir)

// atexit() 专门注册,防止main退出,或者调用exit(), 同时还能释放一些资源

// macro
#define OEY_MAY(x,y)	(	(x) > (y)? (x) : (y) 	)

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

TEST(Idiom, MacroParam) {
	// for param in macro # and ##
	// # force convert block into "block"
#define	OYE_STR(s)	#s
	printf(OYE_STR(abcdefg1234567890));

#define OYE_MERGE(a,b)	(int)(a##40##b)
	printf("\n%d\n",OYE_MERGE(10, 20));	// 104020 
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

