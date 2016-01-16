#include "Tools.h"

BITMAP Tools::GetBmp(LPCTSTR title)
{
	static HBITMAP hbmp = 0;
	hbmp = (HBITMAP)::LoadImage(GetModuleHandle(0), title, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	static BITMAP bmp = { 0 };
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	return bmp;
}

BITMAP Tools::CreateTex(LPCTSTR title)
{
	static HBITMAP hbmp = 0;
	hbmp = (HBITMAP)::LoadImage(GetModuleHandle(0), title, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	static BITMAP bmp = { 0 };

	if (hbmp){
		HDC BufferDC = CreateCompatibleDC(NULL);
		if (BufferDC){
			SelectObject(BufferDC, hbmp);
			HDC DirectDC = CreateCompatibleDC(NULL);
			if (DirectDC){
				BITMAP bm;
				GetObject(hbmp, sizeof(bm), &bm);
				BITMAPINFO RGB32BitsBITMAPINFO;
				ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
				RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				RGB32BitsBITMAPINFO.bmiHeader.biWidth = bm.bmWidth;
				RGB32BitsBITMAPINFO.bmiHeader.biHeight = bm.bmHeight;
				RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
				RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;
				UINT * ptPixels;

				HBITMAP DirectBitmap = CreateDIBSection(DirectDC, (BITMAPINFO *)&RGB32BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);
				if (DirectBitmap){
					HGDIOBJ PreviousObject = SelectObject(DirectDC, DirectBitmap);
					BitBlt(DirectDC, 0, 0, bm.bmWidth, bm.bmHeight, BufferDC, 0, 0, SRCCOPY);
					for (int i = ((bm.bmWidth*bm.bmHeight) - 1); i >= 0; i--){
						COLORREF oldcolor = ptPixels[i];
						int r = GetRValue(oldcolor);
						int g = GetGValue(oldcolor);
						int b = GetBValue(oldcolor);
						int rr = r + (int)p->PerlinNoise_1D((float)i);
						int gg = g + (int)p->PerlinNoise_1D((float)i);
						int bb = b + (int)p->PerlinNoise_1D((float)i);
						COLORREF newcolor = RGB(rr, gg, bb);
						ptPixels[i] = newcolor;

					}
					SelectObject(DirectDC, PreviousObject);
					hbmp = DirectBitmap;
				}
				DeleteDC(DirectDC);
			}
			DeleteDC(BufferDC);
		}
	}
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	return bmp;
}

void Tools::LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap){
	FILE *pFile = NULL;
	pFile = fopen(strName, "rb");

	if (pFile == NULL){
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	fread(pHeightMap, 1, nSize, pFile);
	int result = ferror(pFile);

	if (result){
		MessageBox(NULL, L"Failed To Get Data!", L"Error", MB_OK);
	}
	fclose(pFile);
}

tga Tools::GetTga(char* file){
	FILE *pfile;
	GLubyte tempColor;
	unsigned char bitCount;
	int colorMode;
	long tgaSize;
	unsigned char unCompressHeader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char tgaHeader[12];
	unsigned char header[6];

	tga ret;

	ret.imageHeight = 0;
	ret.imageWidth = 0;
	ret.type = 0;

	if (!file)
		return ret;

	pfile = fopen(file, "rb");
	if (!pfile)
		return ret;

	fread(tgaHeader, 1, sizeof(tgaHeader), pfile);

	if (memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0)
	{
		MessageBox(NULL, L"File Type Error!", L"Erorr", MB_OK);
		fclose(pfile);
		return ret;
	}

	fread(header, 1, sizeof(header), pfile);

	ret.imageWidth = header[1] * 256 + header[0];
	ret.imageHeight = header[3] * 256 + header[2];

	bitCount = header[4];

	colorMode = bitCount / 8;
	tgaSize = ret.imageWidth * ret.imageHeight * colorMode;

	ret.image = (GLubyte *)malloc(tgaSize);;
	fread(ret.image, 1, tgaSize, pfile);

	for (GLuint index = 0; index < int(tgaSize); index += colorMode)
	{
		tempColor = ret.image[index];
		ret.image[index] = ret.image[index + 2];
		ret.image[index + 2] = tempColor;
	}

	fclose(pfile);
	if (colorMode == 3)
		ret.type = GL_RGB;
	else
		ret.type = GL_RGBA;

	return ret;
}

Tools::Tools(){
	p = new Perlin();
}