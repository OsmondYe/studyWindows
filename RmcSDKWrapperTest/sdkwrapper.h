#include "stdafx.h"
#include "SDLError.h"

#define NXSDK_API extern "C" __declspec(dllexport)

#pragma region SDWL_Library_Interface

NXSDK_API DWORD GetSDKVersion();

NXSDK_API DWORD CreateSDKSession(const wchar_t* wcTempPath, HANDLE* phSession);

NXSDK_API DWORD DeleteSDKSession(HANDLE hSession);

#pragma endregion

#pragma region SDWL_Session

NXSDK_API DWORD SDWL_Session_Initialize(HANDLE hSession, 
	const wchar_t* router, const wchar_t* tenant);

NXSDK_API DWORD SDWL_Session_Initialize2(HANDLE hSession, 
	const wchar_t* workingfolder, const wchar_t* router, 
	const wchar_t* tenant);

NXSDK_API DWORD SDWL_Session_SaveSession(HANDLE hSession, const  wchar_t* folder);

NXSDK_API DWORD SDWL_Session_GetCurrentTenant(HANDLE hSession, HANDLE* phTenant);

typedef struct _NXL_LOGIN_COOKIES{
	wchar_t* key;
	wchar_t* values;

}NXL_LOGIN_COOKIES;

NXSDK_API DWORD SDWL_Session_GetLoginParams(HANDLE hSession, 
	wchar_t** ppURL, NXL_LOGIN_COOKIES** ppCookies,size_t* pSize);

NXSDK_API DWORD SDWL_Session_SetLoginRequest(HANDLE hSession, 
	const char* JsonReturn, HANDLE* hUser);

NXSDK_API DWORD SDWL_Session_GetLoginUser(HANDLE hSession, 
	const char* UserEmail, const char* PassCode, HANDLE *hUser);

#pragma endregion //SDWL_Session

#pragma region SDWL_Tenant
NXSDK_API DWORD SDWL_Tenant_GetTenant(HANDLE hTenant, wchar_t** pptenant);

NXSDK_API DWORD SDWL_Tenant_GetRouterURL(HANDLE hTenant, wchar_t** pprouterurl);

NXSDK_API DWORD SDWL_Tenant_GetRMSURL(HANDLE hTenant, wchar_t** pprmsurl);

NXSDK_API DWORD SDWL_Tenant_ReleaseTenant(HANDLE hTenant);

#pragma endregion //SDWL_Tenant

#pragma region SDWL_User
NXSDK_API DWORD SDWL_User_GetUserName(HANDLE hUser, wchar_t** ppname);

NXSDK_API DWORD SDWL_User_GetUserEmail(HANDLE hUser, wchar_t** ppemail);

NXSDK_API DWORD SDWL_User_GetPasscode(HANDLE hUser, char** ppasscode);

NXSDK_API DWORD SDWL_User_ProjectMembershipId(HANDLE hUser, DWORD32 projectId, char** projectMembershipId);

NXSDK_API DWORD SDWL_User_TenantMembershipId(HANDLE hUser, char* tenantId, char** tenantMembershipId);

NXSDK_API DWORD SDWL_User_LogoutUser(HANDLE hUser);

/*skydrm 0; smal 1; google 2; facebook 3; yahoo 4*/
NXSDK_API DWORD SDWL_User_GetUserType(HANDLE hUser, int* type);

NXSDK_API DWORD SDWL_User_UpdateUserInfo(HANDLE hUser);


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

NXSDK_API DWORD SDWL_User_ProtectFile(HANDLE hUser, 
	const wchar_t* path, 
	int* pRights, int len,
	WaterMark watermark, Expiration expiration,
	const char* tags);

NXSDK_API DWORD SDWL_User_ShareFile(HANDLE hUser, 
	const wchar_t* path, 
	int* pRights,int lenRights, 
	const char* recipients[], int lenRecipients, 
	const wchar_t* comments, 
	WaterMark watermark, Expiration expiration, 
	const char* tags);

NXSDK_API DWORD SDWL_User_UpdateRecipients(HANDLE hUser, 
	HANDLE hNxlFile, 
	const char* addmails[], int lenaddmails,
	const char* delmails[], int lendelmails);

NXSDK_API DWORD SDWL_User_GetRecipients(HANDLE hUser, 
	HANDLE hNxlFile, 
	char** emails,int* peSize,
	char** addEmials,int* paeSize,
	char** removEmails,int*preSize);

NXSDK_API DWORD SDWL_User_UploadFile(HANDLE hUser, HANDLE hNxlFile);

NXSDK_API DWORD SDWL_User_OpenFile(HANDLE hUser, 
	const wchar_t* nxl_path,
	HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_User_DecryptNXLFile(HANDLE hUser, 
	HANDLE hNxlFile, 
	const wchar_t* path);

NXSDK_API DWORD SDWL_User_UploadActivityLogs(HANDLE hUser);

// call RMS to fetch
NXSDK_API DWORD SDWL_User_GetHeartBeatInfo(HANDLE hUser);

// must call SDWL_User_GetHeartBeatInfo first
NXSDK_API DWORD SDWL_User_GetPolicyBundle(HANDLE hUser, char* tenantName, char** ppPolicyBundle);

// must call SDWL_User_GetHeartBeatInfo first
NXSDK_API DWORD SDWL_User_GetWaterMarkInfo(HANDLE hUser, WaterMark* pWaterMark);
#pragma pack(push)
#pragma pack(8)
typedef struct _ProjtectInfo {
	DWORD id;
	char* name;
	char* displayname;
	char* description;
	bool owner;
	DWORD64 totalfiles;
	char* tenantid;
}ProjtectInfo;
#pragma pack(pop)

NXSDK_API DWORD SDWL_User_GetProjectsInfo(HANDLE hUser, 
	ProjtectInfo** pProjects, int* pSize);

enum ProjtectFilter {
	All =0,
	OwnedByMe = 1,
	OwnedByOther =2
};
NXSDK_API DWORD SDWL_User_GetListProjtects(HANDLE hUser, 
	uint32_t pagedId, uint32_t pageSize,
	const char* orderBy, ProjtectFilter filter);

NXSDK_API DWORD SDWL_User_ProjectDownloadFile(HANDLE hUser, 
	uint32_t projectId, 
	const wchar_t* pathId,
	const wchar_t* downloadPath, 
	bool bViewOnly);

#pragma pack(push)
#pragma pack(8)
typedef struct _ProjectFileInfo{
	char* id;
	char* duid;
	char* displayPath;
	char* pathId;
	char* nxlName;
}ProjectFileInfo;
#pragma pack(pop)
NXSDK_API DWORD SDWL_User_ProjectListFiles(HANDLE hUser, 
	uint32_t projectId, 
	uint32_t pagedId, 
	uint32_t pageSize,
	const char* orderby, 
	const char* pathId, 
	const char* searchStr, 
	ProjectFileInfo** pplistFiles, 
	uint32_t* plistSize);

#pragma pack(push)
#pragma pack(8)
typedef struct _ProjectClassifacationLables {
	char* name;
	bool multiseclect;
	bool mandatory;
	uint32_t size;
	char* labels; // separate by ;  "a;b;c;d;e;f;g;"
	char* isdefaults;  // 1-true;0-false  separate by ;  "1;0;1;1;1;1"
}ProjectClassifacationLables;
#pragma pack(pop)

NXSDK_API DWORD SDWL_User_ProjectClassifacation(HANDLE hUser, 
	uint32_t projectId, 
	ProjectClassifacationLables** ppProjectClassifacationLables,
	uint32_t* pSize);

#pragma endregion //SDWL_User

#pragma region LocalFiles
//
// Wrapper ISDRFiles
//
NXSDK_API DWORD SDWL_File_GetListNumber(HANDLE hFile,  int* pSize);

NXSDK_API DWORD SDWL_File_GetList(HANDLE hFile, wchar_t** strArray, int* pSize);

NXSDK_API DWORD SDWL_File_GetFile(HANDLE hFile, int index, HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_File_GetFile2(HANDLE hFile, 
	const wchar_t* FileName, HANDLE* hNxlFile);

NXSDK_API DWORD SDWL_File_RemoveFile(HANDLE hFile, HANDLE hNxlFile, bool *pResult);

#pragma endregion //LocalFiles

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

//NXSDK_API DWORD SDWL_NXL_File_GetClassificationSetting(HANDLE hNxlFile, char** ppResult);

NXSDK_API DWORD SDWL_NXL_File_IsUploadToRMS(HANDLE hNxlFile, bool* pResult);

NXSDK_API DWORD SDWL_NXL_File_GetAdhocWatermarkString(HANDLE hNxlFile, char** ppWmStr);

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

NXSDK_API DWORD SDWL_NXL_File_GetActivityInfo(const wchar_t* fileName,
	NXL_FILE_ACTIVITY_INFO** ppInfo,
	DWORD* pSize);

NXSDK_API DWORD SDWL_NXL_File_GetNxlFileActivityLog(HANDLE hNxlFile,
	DWORD64 startPos, DWORD64 count,
	BYTE searchField, 
	const char* searchText,
	BYTE orderByField, 
	bool orderByReverse);

#pragma endregion //NxlFile






