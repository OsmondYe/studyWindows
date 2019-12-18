#include "pch.h"


// my strchr
char* oye_strchr(const char* string, char ch) {

	while ( *string && *string!=ch ){		// c语言str以\0结尾真是个好东西,可以直接用循环判断 (*string) 为0, 循环结束
		++string;
	}
	if (*string == ch) {
		return (char*)string;
	}
	return (NULL);
}

// my last occurrence of ch in str
char* oye_strrchr(const char* str, char ch) {
	const char* begin = str;
	// move str to end
	while (*str++);

	// search toward front
	while (--str != begin && *str != ch);	

	return (*str) == ch ? (char*)str : NULL;
}
// search str2 in str1
char* oye_strstr(const char* str1, const char* str2) {

	char* cp = (char*)str1;
	char *s1, *s2;			// alarm char* s1,s2  XXXXXX s2 is char , not char*

	if (!*str2) {
		return (char*)str1;
	}

	//scan whole str1
	while (*cp)
	{
		s1 = cp;
		s2 = (char*)str2;

		// each pos in str1, check sutstr , 如果真的是相等了,那么 while结束 *s2==NULL
		while (*s1 && *s2 && !(*s2 - *s1)) {
			s1++, s2++;
		}

		if (!*s2) {
			return cp;
		}
		
		cp++;
	}

	return NULL;

}

TEST(OyeAlgo, Count) {

	char str[] = "hello world";

	cout << oye_strchr(str, 'w') << endl;
	cout << oye_strrchr(str, 'w') << endl;
	cout << oye_strstr(str, "worlddgdf") << endl;			// cout<<NULL 这是一个灾难


}

static int c_fib = 0;
unsigned long long fibonacci(int n) {
	++c_fib;
	if (n == 0) return 0;
	if (n == 1)  return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}


TEST(OyeAlgo, Fibonacci) {
	int count=40;
	/*
		鐢―ivide and Conqure鏉ヨВ鍐�
			鎶婇棶棰樺垝鍒嗕负瀛愰棶棰橈紝鏈�灏忕殑瀛愰棶棰樿В宸茬煡锛屾湰璐ㄤ笂灏辨槸涓�涓爲锛� 浠庝笂鍒颁笅鏉ュ鐞�
	*/
	for (int i = 3; i < count; i++) {
		int fib_num = i;
		c_fib = 0;
		cout << "fib_num:" << fib_num << "\tresult:" << fibonacci(fib_num) << "\tc_fib:" << c_fib << endl;
	}

	/*
		鐢―ynamic Programming鏉ヨВ鍐筹紝 鏅哄晢灞傞潰鐨勪紭瓒婃劅锛屾补鐒惰�岀敓
			鏍规嵁DC寰楁潵鐨勭伒鎰燂紝 濡傛灉浠庝笅鍒颁笂鏉ユ�濊�冮棶棰樼殑瑙ｏ紝涓嶅氨甯告暟鏃堕棿鎼炲畾浜�

	*/
	vector<int> fn(count, 0);
	fn[0] = 0;
	fn[1] = 1;
	c_fib = 0;
	for (int i = 2; i < count; i++) {
		fn[i] = fn[i - 1] + fn[i - 2];
		cout << "fib_num:" << i << "\tresult:" << fn[i] << "\tc_fib:" << ++c_fib << endl;
	}

}