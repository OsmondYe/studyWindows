#pragma once
#include "stdafx.h"

//for a commond background erase 
template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd
{
public:
	CPaintBkgnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
	~CPaintBkgnd() { DeleteObject(m_hbrBkgnd); }

	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		CDCHandle dc = (HDC)wParam;
		RECT rcClient;

		pT->GetClientRect(&rcClient);
		FillRect(dc, &rcClient, m_hbrBkgnd);
		
		return 1;    // we painted the background
	}

protected:
	HBRUSH m_hbrBkgnd;
};


HRESULT CatureImageBy(HWND hwnd, CRect rc);

HRESULT CatureImageBy(HDC hdc, CRect rc);

// for multi screen used
inline CRect GetVirtualScreenRect() {
	return CRect(
		GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN)
	);
}

inline CRect GetPrimayScreenRect() {
	return CRect(
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN)
	);
}


inline void CaptureWholeScreens() {
	CDC dc = ::GetDC(NULL);
	CatureImageBy(dc, GetVirtualScreenRect());
}


class RGBEnum{
public:
	RGBEnum(COLORREF b = RGB(0, 0, 0)) :base(b) {}

	COLORREF cur() {
		static byte r = GetRValue(base);
		static byte g = GetGValue(base);
		static byte b = GetBValue(base);

		if (r < 255) {
			r++;
			base = RGB(r, g, b);
			return base;
		}
		r = 0;
		g++;
		if (g < 255) {
			base = RGB(r, g, b);
			return base;
		}
		g = 0;
		b++;
		if (b < 255) {
			base = RGB(r, g, b);
			return base;
		}
		r = b = g = 0;
		base = RGB(r, g, b);
		return base;
		
			
	}


	COLORREF operator++() {
		return cur();
	}

	COLORREF operator++(int) {
		return cur();
	}

	COLORREF base;

};