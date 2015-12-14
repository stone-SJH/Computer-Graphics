#include <Windows.h>
#include "macro.h"
#ifndef TOOL
#define TOOL
class Tools{
public:
	BITMAP GetBmp(LPCTSTR title);
	void setTbTex();

	Tools();
};
#endif