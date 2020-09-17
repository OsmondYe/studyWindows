#include "pch.h"

namespace ocr {
#include "ocr_api.h"
};




void process_ocr(const std::string& src_in, std::string& rt_out) {
	//cout << src_in << endl << rt_out << endl;

	auto path = R"_(C:\OyeOutput\)_";

	ocr::LIBRARY_HANDLE h = ocr::dynamic_load_aocr_library(path);

	ocr::c_com_asprise_ocr_setup(false);

	long long ptrToApi = ocr::c_com_asprise_ocr_start("eng", OCR_SPEED_FAST, NULL, NULL, NULL);

	auto x=ocr::c_com_asprise_ocr_recognize(ptrToApi, src_in.c_str(), -1, -1, -1, -1, -1,
		OCR_RECOGNIZE_TYPE_TEXT, OCR_OUTPUT_FORMAT_PLAINTEXT,
		NULL, NULL, NULL);

	ocr::c_com_asprise_ocr_stop(ptrToApi);

	if (x != NULL) {
		rt_out.assign(x);
	}

}
