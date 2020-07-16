#include "framework.h"
#include "tls.h"


DWORD tls_index = -1;

bool tls_init() {

	if (tls_index == -1) {
		tls_index = ::TlsAlloc();
		return true;
	}

	return false;
	
}

void tls_deinit()
{
	if (tls_index != -1) {
		if (::TlsFree(tls_index)) {
			tls_index = -1;
		}
		else {
			// free failed
		}
	}
	else {
		// has been freed
	}
}

void* tls_get_data()
{
	if (tls_index == -1) {
		return nullptr;
	};
	void* data = ::TlsGetValue(tls_index);
	// if null, 

	return data;
}

bool tls_set_data(void* data)
{
	if (tls_index == -1) {
		return false;
	};

	return ::TlsSetValue(tls_index, data) == TRUE;
}

