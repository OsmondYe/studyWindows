#include "stdafx.h"

#include "utils.h"
#include <bcrypt.h>
#include <functional>


std::string win::str::from_digit(unsigned char c)
{
	static const char* digits = "0123456789ABCDEF";
	std::string rt;
	rt.push_back(digits[(c >> 4) & 0x0F]);
	rt.push_back(digits[c & 0x0F]);
	return rt;
}

std::string win::str::from_digits(unsigned char * buf, size_t buf_len)
{
	std::string rt;
	if (buf == NULL || buf_len == 0) {
		return rt;
	}

	while (0 != buf_len) {
		rt += from_digit(*(buf++));
		buf_len--;
	}
	return rt;
}



bool win::file::is_file_exist(const char * file)
{
	DWORD dwAttrib = GetFileAttributesA(file);
	return (INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool win::file::is_file_exist(const wchar_t * file)
{
	DWORD dwAttrib = GetFileAttributesW(file);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::file::is_dir_exist(const char * dir)
{
	DWORD dwAttrib = GetFileAttributesA(dir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::file::is_dir_exist(const wchar_t * dir)
{
	DWORD dwAttrib = GetFileAttributesW(dir);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool win::file::is_path_exist(const char * dir)
{
	return INVALID_FILE_ATTRIBUTES != GetFileAttributesA(dir);
}

bool win::file::is_path_exist(const wchar_t * dir)
{
	return INVALID_FILE_ATTRIBUTES != GetFileAttributesW(dir);
}


void win::file::del_all_files(std::wstring path)
{
	std::vector<std::wstring::value_type> doubleNullTerminatedPath;
	std::copy(path.begin(), path.end(), std::back_inserter(doubleNullTerminatedPath));
	doubleNullTerminatedPath.push_back(L'\0');
	doubleNullTerminatedPath.push_back(L'\0');

	SHFILEOPSTRUCTW fileOperation;
	fileOperation.wFunc = FO_DELETE;
	fileOperation.pFrom = &doubleNullTerminatedPath[0];
	fileOperation.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION | FOF_SILENT;

	::SHFileOperationW(&fileOperation);
}



HANDLE win::get_process(int process_id) {
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
}

HANDLE win::get_process(const wchar_t * process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return INVALID_HANDLE_VALUE;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
		if (!_tcscmp(pe.szExeFile, process_name)) {
			CloseHandle(hSnapshot);
			return get_process(pe.th32ProcessID);
		}
	}
	return INVALID_HANDLE_VALUE;
}

// some utility classes
namespace {
	class ScopeGuard {
	public:
		explicit ScopeGuard(std::function<void()> onLeaveScope):_onLeaveScope(onLeaveScope), _dismissd(false){}
		~ScopeGuard() { if (!_dismissd) _onLeaveScope(); }
		inline void dismiss() { _dismissd = true; }
	private: // nocopyable
		ScopeGuard(ScopeGuard const&);
		ScopeGuard& operator=(ScopeGuard const&);
	private:
		std::function<void()> _onLeaveScope;
		bool _dismissd;
	};

}



// crypt needed params here
namespace {

	typedef enum PROVIDER_ID {
		PROV_AES = 0,       /**< AES Provider    (0) */
		PROV_RSA,           /**< RSA Provider    (1) */
		PROV_DH,            /**< Diffie-Hellman Provider (2) */
		PROV_MD5,           /**< MD5 Provider    (3) */
		PROV_SHA1,          /**< SHA1 Provider   (4) */
		PROV_SHA256,        /**< SHA256 Provider (5) */
		PROV_HMAC_MD5,      /**< MD5 Provider    (6) */
		PROV_HMAC_SHA1,     /**< SHA1 Provider   (7) */
		PROV_HMAC_SHA256,   /**< SHA256 Provider (8) */
		PROV_MAX            /**< Maximum Provider Id Value  (9) */
	} PROVIDER_ID;

#define IsHashAlg(id)   ((id) >= PROV_MD5 && (id) < PROV_MAX)
#define IsHmacHashAlg(id)   ((id) == PROV_HMAC_MD5 || (id) == PROV_HMAC_SHA1 || (id) == PROV_HMAC_SHA256)


	class Provider
	{
	public:
		Provider(PROVIDER_ID id) : _id(id), _h(NULL), _object_length(0), _hash_length(0) {
			Open();
		}
		virtual ~Provider() {
			Close();
		}

		LPCWSTR GetAlgName() const {
			LPCWSTR name = L"Unknown";
			switch (_id)
			{
			case PROV_AES:
				name = BCRYPT_AES_ALGORITHM;
				break;
			case PROV_RSA:
				name = BCRYPT_RSA_ALGORITHM;
				break;
			case PROV_DH:
				name = BCRYPT_DH_ALGORITHM;
				break;
			case PROV_MD5:
				name = BCRYPT_MD5_ALGORITHM;
				break;
			case PROV_SHA1:
				name = BCRYPT_SHA1_ALGORITHM;
				break;
			case PROV_SHA256:
				name = BCRYPT_SHA256_ALGORITHM;
				break;
			case PROV_HMAC_MD5:
				name = BCRYPT_MD5_ALGORITHM;
				break;
			case PROV_HMAC_SHA1:
				name = BCRYPT_SHA1_ALGORITHM;
				break;
			case PROV_HMAC_SHA256:
				name = BCRYPT_SHA256_ALGORITHM;
				break;
			default:
				break;
			}
			return name;
		}
		inline void Close() {
			if (_h) {
				(VOID)BCryptCloseAlgorithmProvider(_h, 0);
				_h = NULL;
				_object_length = 0;
				_hash_length = 0;
			}
		}

		inline bool Opened() const { return (NULL != _h); }
		inline operator BCRYPT_ALG_HANDLE() const { return _h; }
		inline ULONG GetObjectLength() const { return _object_length; }
		inline ULONG GetHashDataLength() const { return _hash_length; }

	protected:
		void Open() {
			LONG Status = 0;

			assert(NULL == _h);
			assert(_id < PROV_MAX);

			do {

				Status = BCryptOpenAlgorithmProvider(&_h, GetAlgName(), MS_PRIMITIVE_PROVIDER, IsHmacHashAlg(_id) ? BCRYPT_ALG_HANDLE_HMAC_FLAG : 0);
				if (Status != 0) {
					break;
				}

				if (IsHashAlg(_id)) {

					unsigned long returned_length = 0;

					Status = BCryptGetProperty(_h, BCRYPT_OBJECT_LENGTH, (PUCHAR)&_object_length, sizeof(ULONG), &returned_length, 0);
					if (Status != 0) {
						_object_length = 0;
						break;
					}

					Status = BCryptGetProperty(_h, BCRYPT_HASH_LENGTH, (PUCHAR)&_hash_length, sizeof(ULONG), &returned_length, 0);
					if (Status != 0) {
						break;
					}
				}

			} while (FALSE);

			if (0 != Status) {
				// Error happened
				Close();
			}
		}

	private:
		// Copy is not allowed
		Provider(const Provider& rhs) {}

	private:
		PROVIDER_ID         _id;
		BCRYPT_ALG_HANDLE   _h;
		unsigned long       _object_length;
		unsigned long       _hash_length;
	};

	class ProvManager
	{
	public:
		ProvManager(){
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_AES)));			// AES
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_RSA)));			// RSA
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_DH)));				// Diffie-Hellman
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_MD5)));			// MD5
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_SHA1)));			// SHA1
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_SHA256)));			// SHA256
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_HMAC_MD5)));       // HMAC_MD5
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_HMAC_SHA1)));		// HMAC_SHA1
			_providers.push_back(std::shared_ptr<Provider>(new Provider(PROV_HMAC_SHA256)));	// HMAC_SHA256
			assert(PROV_MAX == _providers.size());
		}

		~ProvManager() {
		}

		inline void clear() { _providers.clear(); }
		inline Provider* GetProvider(PROVIDER_ID id) const { return (id < PROV_MAX) ? _providers[id].get() : NULL; }

	private:
		std::vector<std::shared_ptr<Provider>> _providers;
	};

	static ProvManager PROVIDERMGR;


	Provider* GetProvider(PROVIDER_ID id)
	{
		return PROVIDERMGR.GetProvider(id);
	}
	
}

void win::crypt::sha1(const unsigned char * data, size_t data_len, unsigned char * out_buf)
{
	// sanity check
	if (NULL == data || 0 == data_len || NULL == out_buf) {
		return;
	}

	Provider* prov = GetProvider(PROV_SHA1);
	if (NULL == prov) {
		return;
	}

	if (!prov->Opened()) {
		return;
	}
	// do
	NTSTATUS status = 0;
	BCRYPT_HASH_HANDLE h = NULL;
	ScopeGuard s([&]() {if (h) { ::BCryptDestroyHash(h); h = NULL; }});
	
	status=::BCryptCreateHash(*prov, &h, NULL, 0, NULL, 0, 0);
	if (0 != status) {
		return;
	}

	status = ::BCryptHashData(h, (PUCHAR)data, data_len, 0);
	if (0 != status) {
		return;
	}

	status = ::BCryptFinishHash(h, out_buf, 20, 0);
	if (0 != status) {
		return;
	}
}

