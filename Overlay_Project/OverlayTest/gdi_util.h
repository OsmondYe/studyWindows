#pragma once


namespace gdi {

	std::vector<std::wstring> GetInstalledFonts();


	bool SaveFileAsBitmap(Gdiplus::Image* image, std::wstring path);

	
	Gdiplus::PointF CaculateRotated(Gdiplus::PointF& org, int angle);


	Gdiplus::RectF CaculateOutbound(Gdiplus::PointF (&org)[4]);

}