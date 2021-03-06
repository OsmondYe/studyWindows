
#include <string>




typedef struct _SIMPLESTRUCT {
	char	byteValue;
	short	shortValue;
	int		intValue;
	int64_t int64Value;
	bool	boolValue;
	float	floatValue;
	double	doubleValue;
} SIMPLESTRUCT, *PSIMPLESTRUCT;


DLL_FUN_EXPORT int add(int a, int b);
DLL_FUN_EXPORT int sub(int a, int b);


DLL_FUN_EXPORT void UntypedParamShow(void* data, int flag);


//
//  String test
//
DLL_FUN_EXPORT void StrReverseA(char* in, char* out);
DLL_FUN_EXPORT void StrReverseW(wchar_t* in, wchar_t* out);

DLL_FUN_EXPORT wchar_t* GetStringFromHeap();
DLL_FUN_EXPORT void FreeStringFromHeap(wchar_t* p);


DLL_FUN_EXPORT wchar_t* GetStringFromCOMMem();


DLL_FUN_EXPORT void FillStringFromHeap(const wchar_t* pOrg, wchar_t** pOut);
DLL_FUN_EXPORT void Free_FillStringFromHeap(wchar_t* pIn);

DLL_FUN_EXPORT void FillStringFromComMem(const wchar_t* pOrg, wchar_t** pOut);

DLL_FUN_EXPORT void ReturnStringArrays(wchar_t** pArray, int* pSize);

//
//  struct
//
DLL_FUN_EXPORT void FillStruct(PSIMPLESTRUCT pSt);
DLL_FUN_EXPORT PSIMPLESTRUCT ReturnStructByNew();
DLL_FUN_EXPORT void FreeStructByNewed(PSIMPLESTRUCT pSt);
DLL_FUN_EXPORT PSIMPLESTRUCT ReturnStructByComMem();
DLL_FUN_EXPORT void ReturnStructAsParam(PSIMPLESTRUCT* ppSt);



