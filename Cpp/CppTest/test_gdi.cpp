#include "stdafx.h"

HANDLE DDBtoDIB(HBITMAP bitmap, DWORD dwCompression, HPALETTE  hPal, DWORD* sizeimage) {
    BITMAP            bm;
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER     lpbi;
    DWORD            dwLen;
    HANDLE            hDib;
    HANDLE            handle;
    HDC             hdc;

    //不支持BI_BITFIELDS类型
    if (dwCompression == BI_BITFIELDS)
        return NULL;

    //如果调色板为空，则用默认调色板
    if (hPal == NULL)
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    //获取位图信息
    GetObject(bitmap, sizeof(bm), (LPSTR)&bm);

    //初始化位图信息头
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = bm.bmPlanes * bm.bmBitsPixel;
    bi.biCompression = dwCompression;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    //计算信息头及颜色表大小
    int ncolors = (1 << bi.biBitCount); if (ncolors > 256)
        ncolors = 0;
    dwLen = bi.biSize + ncolors * sizeof(RGBQUAD);

    // we need a device context to get the dib from
    hdc = GetDC(NULL);
    hPal = SelectPalette(hdc, hPal, FALSE);
    RealizePalette(hdc);

    //为信息头及颜色表分配内存
    hDib = GlobalAlloc(GMEM_FIXED, dwLen);

    if (!hDib) {
        SelectPalette(hdc, hPal, FALSE);
        ReleaseDC(NULL, hdc);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDib;
    *lpbi = bi;
    //调用 GetDIBits 计算图像大小
    GetDIBits(hdc, bitmap, 0L, (DWORD)bi.biHeight,
        (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

    bi = *lpbi;
    //图像的每一行都对齐(32bit)边界
    if (bi.biSizeImage == 0) {
        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
            * bi.biHeight;
        if (dwCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    //重新分配内存大小，以便放下所有数据
    dwLen += bi.biSizeImage;
    if (handle = GlobalReAlloc(hDib, dwLen, GMEM_MOVEABLE))
        hDib = handle;
    else {
        GlobalFree(hDib);

        //重选原始调色板
        SelectPalette(hdc, hPal, FALSE);
        ReleaseDC(NULL, hdc);
        return NULL;
    }

    //获取位图数据
    lpbi = (LPBITMAPINFOHEADER)hDib;

    //最终获得的DIB
    BOOL bgotbits = GetDIBits(hdc, bitmap,
        0L,                      //扫描行起始处
        (DWORD)bi.biHeight,      //扫描行数
        (LPBYTE)lpbi             //位图数据地址
        + (bi.biSize + ncolors * sizeof(RGBQUAD)),
        (LPBITMAPINFO)lpbi,      //位图信息地址
        (DWORD)DIB_RGB_COLORS);  //颜色板使用RGB

    if (!bgotbits)
    {
        GlobalFree(hDib);

        SelectPalette(hdc, hPal, FALSE);
        ReleaseDC(NULL, hdc);
        return NULL;
    }

    SelectPalette(hdc, hPal, FALSE);
    ReleaseDC(NULL, hdc);
    *sizeimage = bi.biSizeImage;
    return hDib;
}

bool screen_cap(const char* FileName) {
	HDC hdc = ::CreateDC(L"DISPLAY", NULL, NULL, NULL);
	HDC hCompatibleHDC = ::CreateCompatibleDC(hdc);
	HBITMAP BmpScreen = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES));

	SelectObject(hCompatibleHDC, BmpScreen);
	BitBlt(hCompatibleHDC, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), hdc, 0, 0, SRCCOPY);

	DWORD sizeimage;
	HANDLE	pbitmapwithoutfileh = DDBtoDIB(BmpScreen, BI_RGB, 0, &sizeimage);

	BITMAPFILEHEADER bfh;
	//设置位图信息头结构
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = 54 + sizeimage;
	bfh.bfOffBits = 54;


	HANDLE  hFile = CreateFileA(FileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWrite;

	// 写入位图文件头
	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
	// 写入位图文件其余内容
	WriteFile(hFile, pbitmapwithoutfileh, bfh.bfSize, &dwWrite, NULL);

    ::CloseHandle(hFile);

    return true;

}

TEST(gdi, ScreenCapture) {
    screen_cap("d:\\desktop.bmp");
}