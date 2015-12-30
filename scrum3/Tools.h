#include <Windows.h>
#include "macro.h"
#ifndef _TOOL_
#define _TOOL_
class Tools{
public:
	BITMAP GetBmp(LPCTSTR title);
	void setTbTex();

	Tools();
};
#endif