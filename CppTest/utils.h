#pragma once

// ns win means: dependend on windows system libraries
namespace win {
	bool is_file_exist(const char* file);
	bool is_file_exist(const wchar_t* file);

	bool is_dir_exist(const char* dir);
	bool is_dir_exist(const wchar_t* dir);

	bool is_path_exist(const char* dir);
	bool is_path_exist(const wchar_t* dir);

	HANDLE get_process(const wchar_t* process_name);
	HANDLE get_process(int process_id);

namespace crypt {
	void sha1(const unsigned char* data,
		size_t data_len,
		unsigned char* out_buf  // 20byes as sha1 required
	);
}
};
