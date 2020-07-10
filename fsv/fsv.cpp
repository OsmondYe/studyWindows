// fsv.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "fsv.h"


// This is an example of an exported variable
FSV_API int nfsv=0;

// This is an example of an exported function.
FSV_API int fnfsv(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cfsv::Cfsv()
{
    return;
}
