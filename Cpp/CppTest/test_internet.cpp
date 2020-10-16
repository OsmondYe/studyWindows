#include "stdafx.h"

#include <urlmon.h>  // URLDownlaodToFile

#pragma comment(lib, "Urlmon.lib")


TEST(INet, DownloadFile) {

	// seem only http can do that,  if https, it will failed

	const wchar_t* pInetFile =
		LR"_(http://www.dapenti.com/blog/cdwj.gif)_"
		;
	const wchar_t* pLocalFile = LR"_(D:\xxx.gif)_";

	auto rt= ::URLDownloadToFileW(0, pInetFile, pLocalFile, 0, 0);

}