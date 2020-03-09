#include "pch.h"
/*
c11  alias_declaration is a upgrade weapon for typedef
typedef  vs  using X =

*/

// which is good?
typedef std::unique_ptr < std::unordered_map<std::string, std::string>>  UPtrMapSS;
using UptrUMSS = std::unique_ptr < std::unordered_map<std::string, std::string>>;

//
typedef void (*FP)(int, std::string);
using FP2 = void (*)(int, std::string);


// alias declaration can be templatized; while typedef can't
template<typename T>
using MyList = std::list<T>;

// great using in template class when using template template param

