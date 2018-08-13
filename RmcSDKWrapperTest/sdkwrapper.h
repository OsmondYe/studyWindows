#include "stdafx.h"
#include "SDLError.h"

#define NXSDK_API extern "C" __declspec(dllexport)

#pragma region SDWL_Library_Interface

NXSDK_API DWORD GetSDKVersion();

NXSDK_API DWORD CreateSDKSession(wchar_t* wcTempPath, HANDLE* phSession);

NXSDK_API DWORD DeleteSDKSession(HANDLE hSession);
#pragma endregion

#pragma region SDWL_Session

NXSDK_API DWORD SDWL_Session_Initialize(HANDLE hSession, wchar_t* router, wchar_t* tenant);

NXSDK_API DWORD SDWL_Session_Initialize2(HANDLE hSession, wchar_t* workingfolder, wchar_t* router, wchar_t* tenant);

NXSDK_API DWORD SDWL_Session_SaveSession(HANDLE hSession,wchar_t* folder);

NXSDK_API DWORD SDWL_Session_GetCurrentTenant(HANDLE hSession, HANDLE* phTenant);

typedef struct _NXL_LOGIN_COOKIES{
	wchar_t* key;
	wchar_t* values;

}NXL_LOGIN_COOKIES;

NXSDK_API DWORD SDWL_Session_GetLoginCookies(HANDLE hSession, NXL_LOGIN_COOKIES** ppCookies,size_t* pSize);

NXSDK_API DWORD SDWL_Session_SetLoginRequest(HANDLE hSession, char* JsonReturn, HANDLE* hUser);

NXSDK_API DWORD SDWL_Session_GetLoginUser(HANDLE hSession, char* UserEmail, char* PassCode, HANDLE *hUser);

#pragma endregion

#pragma region SDWL_Tenant
NXSDK_API DWORD SDWL_Tenant_GetTenant(HANDLE hTenant, wchar_t** pptenant);

NXSDK_API DWORD SDWL_Tenant_GetRouterURL(HANDLE hTenant, wchar_t** pprouterurl);

NXSDK_API DWORD SDWL_Tenant_GetRMSURL(HANDLE hTenant, wchar_t** pprmsurl);

NXSDK_API DWORD SDWL_Tenant_ReleaseTenant(HANDLE hTenant);

#pragma endregion

#pragma region SDWL_User
NXSDK_API DWORD SDWL_User_GetUserName(HANDLE hUser, wchar_t** ppname);

NXSDK_API DWORD SDWL_User_GetUserEmail(HANDLE hUser, wchar_t** ppemail);

NXSDK_API DWORD SDWL_User_LogoutUser(HANDLE hUser);
/*
	skydrm 0;
	smal, 1
	google ,2
	facebook,3
	yahoo,4
*/
NXSDK_API DWORD SDWL_User_GetUserType(HANDLE hUser, int* type);

NXSDK_API DWORD SDWL_User_UpdateUserInfo(HANDLE hUser);

NXSDK_API DWORD SDWL_User_GetPasscode(HANDLE hUser, char** ppasscode);

NXSDK_API DWORD SDWL_User_UpdateMyDriveInfo(HANDLE hUser);

NXSDK_API DWORD SDWL_User_GetMyDriveInfo(HANDLE hUser, DWORD64* usage, DWORD64* total);

NXSDK_API DWORD SDWL_User_GetLocalFile(HANDLE hUser, HANDLE* hLocalFiles);
#pragma pack(push)
#pragma pack(8)
typedef struct _WaterMark {
	char* text;
	char* fontName;
	char* fontColor;
	int fontSize;
	int transparency;
	int rotation;
	bool repeat;
}WaterMark;
#pragma pack(pop)

enum ExpiryType {
	never = 0,
	relative,
	absolute,
	range
};

#pragma pack(push)
#pragma pack(8)
typedef struct _Expiration {
	ExpiryType type;
	DWORD64  start;
	DWORD64  end;
}Expiration;
#pragma pack(pop)

NXSDK_API DWORD SDWL_User_ProtectFile(HANDLE hUser, wchar_t* path, int* pRights, int len,WaterMark watermark, Expiration expiration,char* tags);

NXSDK_API DWORD SDWL_User_ShareFile(HANDLE hUser, wchar_t* path, int* pRights,int lenRights, char* recipients[], int lenRecipients, wchar_t* comments, WaterMark watermark, Expiration expiration, char* tags);

NXSDK_API DWORD SDWL_User_UpdateRecipients(HANDLE hUser, HANDLE hNxlFile, char* addmails[], int lenaddmails, char* delmails[], int lendelmails);

NXSDK_API DWORD SDWL_User_UploadFile(HANDLE hUser, HANDLE hNxlFile);

NXSDK_API DWORD SDWL_User_OpenFile(HANDLE hUser, wchar_t* nxl_path, HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_User_DecryptNXLFile(HANDLE hUser, HANDLE hNxlFile, wchar_t* path);

NXSDK_API DWORD SDWL_User_UploadActivityLogs(HANDLE hUser);






NXSDK_API DWORD SDWL_User_GetWaterMarkInfo(HANDLE hUser, WaterMark* pWaterMark);

#pragma endregion

#pragma region LocalFiles
//
// Wrapper ISDRFiles
//
NXSDK_API DWORD SDWL_File_GetListNumber(HANDLE hFile,  int* pSize);

NXSDK_API DWORD SDWL_File_GetList(HANDLE hFile, wchar_t** strArray, int* pSize);

NXSDK_API DWORD SDWL_File_GetFile(HANDLE hFile, int index, HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_File_GetFile2(HANDLE hFile, wchar_t* FileName, HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_File_RemoveFile(HANDLE hFile, HANDLE hNxlFile, bool *pResult);

#pragma endregion

#pragma region NxlFile
//
// Wrapper ISDRNXLFile
//
NXSDK_API DWORD SDWL_NXL_File_GetFileName(HANDLE hNxlFile, wchar_t** ppname);

NXSDK_API DWORD SDWL_NXL_File_GetFileSize(HANDLE hNxlFile, DWORD64* pSize);

NXSDK_API DWORD SDWL_NXL_File_IsValidNxl(HANDLE hNxlFile, bool* pResult );

NXSDK_API DWORD SDWL_NXL_File_GetRights(HANDLE hNxlFile, int** pprights, int* pLen );

NXSDK_API DWORD SDWL_NXL_File_GetWaterMark(HANDLE hNxlFile, WaterMark* pWaterMark);

NXSDK_API DWORD SDWL_NXL_File_GetExpiration(HANDLE hNxlFile, Expiration* pExpiration);

NXSDK_API DWORD SDWL_NXL_File_GetTags(HANDLE hNxlFile, char** ppTags);

NXSDK_API DWORD SDWL_NXL_File_CheckRights(HANDLE hNxlFile, int right, bool* pResult);

NXSDK_API DWORD SDWL_NXL_File_GetClassificationSetting(HANDLE hNxlFile, char** ppResult);

NXSDK_API DWORD SDWL_NXL_File_IsUploadToRMS(HANDLE hNxlFile, bool* pResult);

#pragma pack(push)
#pragma pack(8)
typedef struct _NXL_FILE_ACTIVITY_INFO {
	char* duid;
	char* email;
	char* operation;
	char* deviceType;
	char* deviceId;
	char* accessResult;
	DWORD64 accessTime;
} NXL_FILE_ACTIVITY_INFO;
#pragma pack(pop)

NXSDK_API DWORD SDWL_NXL_File_GetActivityInfo(wchar_t* fileName,
	NXL_FILE_ACTIVITY_INFO** ppInfo,
	DWORD* pSize);

NXSDK_API DWORD SDWL_NXL_File_GetNxlFileActivityLog(HANDLE hNxlFile,
	DWORD64 startPos, DWORD64 count,
	BYTE searchField, char* searchText,
	BYTE orderByField, bool orderByReverse);

#pragma endregion






