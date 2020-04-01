#include "pch.h"

/*
Discription:
	
*/



void list_all_dll() {
	
}

// tbd, find form PE struct
void list_export_functions(const string& path) {

	auto hm = ::LoadLibraryA(path.c_str());

	//::GetProcAddress()
}