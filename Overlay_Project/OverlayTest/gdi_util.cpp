#include "stdafx.h"
#include "gdi_util.h"

#include <cmath>  // using sin cos

using namespace std;
using namespace Gdiplus;

const double PI = std::acos(-1);

namespace{
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
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

namespace gdi {

	vector<wstring> GetInstalledFonts()
	{
		Gdiplus::InstalledFontCollection ifc;
		vector<wstring> rt;
		auto count = ifc.GetFamilyCount();
		int actualFound = 0;

		Gdiplus::FontFamily* buf = new Gdiplus::FontFamily[count];
		ifc.GetFamilies(count, buf, &actualFound);
		for (int i = 0; i < actualFound; i++) {
			wchar_t name[0x20] = { 0 };
			buf[i].GetFamilyName(name);
			rt.push_back(name);
		}

		delete[] buf;
		return rt;
	}

	bool SaveFileAsBitmap(Gdiplus::Image * image, std::wstring path)
	{

		CLSID clsid;
		GetEncoderClsid(L"image/bmp", &clsid);

		image->Save(path.c_str(), &clsid);

		return false;
	}

	Gdiplus::PointF CaculateRotated(Gdiplus::PointF& org, int angle)
	{
		Gdiplus::PointF rt;

		double radians = angle * PI / 180;
		
		rt.X = org.X*std::cos(radians) - org.Y*std::sin(radians);
		rt.Y = org.X*std::sin(radians) + org.Y*std::cos(radians);

		return rt;
	}


	// 给定四个点 如何计算 最小矩形正好包含所有信息?
	// 最小的x,y是原点,  最小的x和最大的x的差值就是宽
	Gdiplus::RectF CaculateOutbound(Gdiplus::PointF(&org)[4])
	{

		vector<Gdiplus::REAL> Xs,Ys;
		for (int i = 0; i < 4; i++) {
			Xs.push_back(org[i].X);
			Ys.push_back(org[i].Y);
		}

		std::sort(Xs.begin(), Xs.end());
		std::sort(Ys.begin(), Ys.end());
		
		Gdiplus::REAL width = Xs.back() - Xs.front();
		Gdiplus::REAL height = Ys.back() - Ys.front();

		return Gdiplus::RectF(Xs.front(),Ys.front(),width,height);
	}


}