#include "Tools.h"

BITMAP Tools::GetBmp(LPCTSTR title)
{
	static HBITMAP hbmp = 0;
	hbmp = (HBITMAP)::LoadImage(GetModuleHandle(0), title, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	static BITMAP bmp = { 0 };
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	return bmp;
}