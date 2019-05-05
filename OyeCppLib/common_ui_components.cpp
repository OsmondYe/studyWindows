#include "stdafx.h"
#include "common_ui_components.h"
#include <gdiplus.h>

using namespace Gdiplus;

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


HRESULT CatureImageBy(HWND hwnd, CRect rc)
{
	HRESULT rt = S_OK;
	CDC targetdc(::GetDC(hwnd));
	

	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));

	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = rc.Width();
	Info.bmiHeader.biHeight = rc.Height();
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = 24;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = 0;
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);



	// crate a memdc with same size of target window
	CMemoryDC memdc(targetdc, rc);
	
	char* Pixels = NULL;
	memdc.m_bmp.Attach(CreateDIBSection(memdc, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0));
	memdc.SelectBitmap(memdc.m_bmp);
	   	 
	// copy hwnd's image into a memdc
	if (!memdc.BitBlt(0, 0, rc.Width(), rc.Height(), targetdc, 0, 0, SRCCOPY)) {
		return S_FALSE;
	}


	std::fstream hFile("d:\\allTestFile\\osmond.bmp", 
		std::ios::out | std::ios::binary
	);
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((24 * rc.Width() + 31) & ~31) / 8) * rc.Height());
		hFile.close();
		return true;
	}

}

HRESULT CatureImageBy(HDC hdc, CRect rc)
{
	HRESULT rt = S_OK;
	CDCHandle targetdc(hdc);	
	// crate a memdc with same size of target window
	CMemoryDC memdc(targetdc, rc);

	if (!memdc.BitBlt(0, 0, rc.Width(), rc.Height(), targetdc, 0, 0, SRCCOPY)) {
		return S_FALSE;
	}

	CLSID clsidBmpEncoder;
	GetEncoderClsid(L"image/bmp", &clsidBmpEncoder);
	Gdiplus::Bitmap::FromHBITMAP(memdc.m_bmp, NULL)
		->Save(L"d:\\allTestFile\\osmond.bmp", &clsidBmpEncoder);

	return S_OK;

}
