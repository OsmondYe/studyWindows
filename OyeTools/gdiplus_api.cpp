#include "pch.h"

#include <ctime>
#include <sstream>

#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")


namespace {
	using namespace Gdiplus;
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}

}
ULONG_PTR gGidplusToken;
Gdiplus::GdiplusStartupInput gGdipulsInput;


struct param {
	string in; // in
	string out; // in 
	// out
	wstring src;
	wstring dst;
	wstring gdi_format;
	CLSID  gdi_clsid;
};

void param_process(param& p) {
	p.src.assign(p.in.begin(), p.in.end());
	if (!p.dst.empty()) {
		p.dst.assign(p.out.begin(), p.out.end());
	}
	else {
		string folder = p.in.substr(0, p.in.find_last_of('\\'));
		string name = p.in.substr(p.in.find_last_of('\\')+1);
		string ext = name.substr(name.find_last_of('.')+1);
		// out format: out_hh_mm_ss.ext;
		std::time_t t = std::time(0);
		std::tm ts;
		localtime_s(&ts,&t);

		ostringstream oss;
		oss << name.substr(0, name.find_last_of('.'))<<"_out_" << ts.tm_hour << "_" << ts.tm_min << "_" << ts.tm_sec << ext;

		name = oss.str();
		
		string out_path = folder + '\\' + name + '.' + ext;
		p.dst.assign(out_path.begin(), out_path.end());
	}
	p.gdi_format = L"image/jpeg";
	GetEncoderClsid(p.gdi_format.c_str(), &p.gdi_clsid);
}



// using GDI+ to gray a picture
void process_picture_gray(const std::string& in, const std::string& out)
{
	process_picture_gray(in, out, 0, 0, 0);
	
}

auto op_0 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG();int b = c.GetB();
	int x = (int)(0.299f * r + 0.587f * g + 0.114f * b);
	return Gdiplus::Color(x, x, x);
};

auto op_1 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	int x = (r + g + b) / 3;
	return Gdiplus::Color(x, x, x);
};

auto op_2 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	return Gdiplus::Color(r,0,0);
};

auto op_3 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	return Gdiplus::Color(0,g,0);
};

auto op_4 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	return Gdiplus::Color(0,0,b);
};

// color boost
auto op_5 = [](const Gdiplus::Color& c, float factor) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	r = (int)(r * factor) % 255;
	g = (int)(g * factor) % 255;
	b = (int)(b * factor) % 255;
	return Gdiplus::Color(r,g,b);
};

// color to black & white
auto op_6 = [](const Gdiplus::Color& c, int threshold) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	int x = (int)(0.299f * r + 0.587f * g + 0.114f * b);
	//
	x = x > threshold ? 0xff : 0x00;
	return Gdiplus::Color(x, x, x);
};


// color to black & white
auto op_7 = [](const Gdiplus::Color& c) {
	int r = c.GetR();	int g = c.GetG(); int b = c.GetB();
	r = ~(unsigned char)(r);
	g = ~(unsigned char)(g);
	b = ~(unsigned char)(b);
	return Gdiplus::Color(r,g,b);
};

void process_picture_gray(const std::string& in, const std::string& out, int operation, int op_param_a, float op_param_b) {
	// init gdi++
	Gdiplus::GdiplusStartup(&gGidplusToken, &gGdipulsInput, NULL);
	param p{ in,out };
	param_process(p);

	Gdiplus::Bitmap bp(p.src.c_str());

	if (bp.GetLastStatus() != Gdiplus::Status::Ok) {
		cerr << "Load picture failed" << endl;
		exit(1);
	}

	int w = bp.GetWidth();
	int h = bp.GetHeight();

	for (int i = 0; i <= w; i++) {
		for (int j = 0; j <= h; j++) {
			Gdiplus::Color c;
			bp.GetPixel(i, j, &c);
			// transform			
			Gdiplus::Color n(0, 0, 0, 0);

			switch (operation)
			{
			case 0:		n = op_0(c); break;
			case 1:		n = op_1(c); break;
			case 2:		n = op_2(c); break;
			case 3:		n = op_3(c); break; 
			case 4:		n = op_4(c); break;
			case 5:		n = op_5(c,op_param_b); break;
			case 6:		n = op_6(c,op_param_a); break;
			case 7:		n = op_7(c); break;
			default:
				cerr << "error operation" << endl;
				exit(1);
				break;
			}


			bp.SetPixel(i, j, n);
		}
	}
	// write to output file
	bp.Save(p.dst.c_str(), &p.gdi_clsid);
}