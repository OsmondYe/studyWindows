/*
	record and collect handy functions about string operations
*/
#pragma once
#include <string>
#include <algorithm>
#include <cctype>


namespace oye_str {

	using namespace std;

	auto lamba_ignore_case_compare = [](char l, char r) { return std::toupper(l) == std::toupper(r); };
	auto lamba_ignore_case_compare_w = [](wchar_t l, wchar_t r) { return std::toupper(l) == std::toupper(r); };

	auto lamda_is_white_char = [](char c) { return c == ' ' || c == '\t' || c == '\n'; };
	auto lamda_is_white_char_w = [](wchar_t c) { return c == L' ' || c == L'\t' || c == L'\n'; };

	
	inline bool is_istarts_with() {
		return false;
	}

	inline bool ibegin_with(const wstring& m, const wstring& s) {
		auto m_b = m.cbegin();
		auto s_b = s.cbegin();
		for (; m_b != m.cend() && s_b != s.cend();m_b++,s_b++) {
			if (tolower(*m_b++) != tolower(*s_b++)) {
				return m_b == m.cbegin() ? false : true;
			}
		}
		return true;
	}

	inline bool is_iends_with() {
		return false;
	}

	inline bool iequal(string s1, string s2) {
		if (s1.size() != s2.size()) {
			return false;
		}
		return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), lamba_ignore_case_compare);
	}

	// if s is a sub of m
	inline bool icontain(string m, string s) {
		return m.end()!=std::search(m.begin(), m.end(), s.begin(), s.end(), lamba_ignore_case_compare);
	}


	template< typename Unary_Predicate>
	inline string trim_left_copy_if(const string& s, Unary_Predicate predicate = lamda_is_white_char) {
		auto it = std::find_if_not(s.cbegin(), s.cend(), predicate);
		if (it == s.cend()) {
			return s;
		}
		return std::string(it, s.cend());
	}

	template< typename Unary_Predicate>
	inline string trim_right_copy_if(const string& s, Unary_Predicate predicate = lamda_is_white_char) {
		auto it = std::find_if_not(s.crbegin(), s.crend(), predicate);
		if (it == s.crend()) {
			return s;
		}
		return std::string(s.begin(), it.base());
	}

	template< typename Unary_Predicate>
	inline string trim_copy_if(const string& s, Unary_Predicate predicate = lamda_is_white_char) {
		return trim_right_copy_if(trim_left_copy_if(s, predicate), predicate);
	}

	// trim
	inline string trim_left_copy(const string& s) {
		return trim_left_copy_if(s, lamda_is_white_char);
	}

	inline string trim_right_copy(const string& s) {
		return trim_right_copy_if(s, lamda_is_white_char);
	}

	inline string trim_copy(const string& s) {
		return  trim_copy_if(s, lamda_is_white_char);
	}
};