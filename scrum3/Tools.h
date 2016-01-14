#define _CRT_SECURE_NO_DEPRECATE
#include <Windows.h>
#include <stdio.h>
#include "macro.h"
#include "Perlin.h"
#ifndef _TOOL_
#define _TOOL_
struct tga{
	int imageWidth;
	int imageHeight;
	GLubyte *image;
	unsigned int type;
};
class Tools{
private:
	Perlin* p;
public:
	BITMAP GetBmp(LPCTSTR title);
	BITMAP CreateTex(LPCTSTR title);
	void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap);
	tga GetTga(char* fileName);
	Tools();
};
#endif