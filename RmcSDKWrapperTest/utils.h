#pragma once

// ns win means: dependend on windows system libraries
namespace win {

namespace str {
	// 0x12 to "12"
	std::string from_digit(unsigned char c);
	// 0x123456789aabbccddeeff  to "123456789aabbccddeeff"
	std::string from_digits(unsigned char* buf, size_t buf_len);
}

namespace file {
	bool is_file_exist(const char* file);
	bool is_file_exist(const wchar_t* file);

	bool is_dir_exist(const char* dir);
	bool is_dir_exist(const wchar_t* dir);

	bool is_path_exist(const char* dir);
	bool is_path_exist(const wchar_t* dir);
}
	HANDLE get_process(const wchar_t* process_name);
	HANDLE get_process(int process_id);

namespace crypt {
	void sha1(const unsigned char* data,
		size_t data_len,
		unsigned char* out_buf  // 20 byes as sha1 required
	);
}
};
