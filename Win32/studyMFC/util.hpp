#pragma once

#include "stdafx.h"



namespace util{

	inline Gdiplus::RectF fromCRect(const CRect& rc) {
		return RectF(rc.left, rc.top, rc.Width(), rc.Height());
	}

	inline void setCommonParam(Graphics& g) {
		using Gdiplus::SmoothingMode;
		//g.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
	}


}