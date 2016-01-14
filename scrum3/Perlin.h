#ifndef _PERLIN_
#define _PERLIN_

#include <math.h>

class Perlin{
private:
	float persistence;
	int Number_Of_Octaves;

	void init();
	//插值函数
	float Cosine_Interpolate(float a, float b, float x);

	//一维Perlin函数
	float Noise0(int x);
	float SmoothNoise_0(float x);
	float InterpolatedNoise_0(float x);

	//二维Perlin函数
	float Noise1(int x, int y);
	float SmoothNoise_1(int x, int y);
	float InterpolatedNoise_1(float x, float y);

public:
	Perlin();
	float PerlinNoise_1D(float x);
	float PerlinNoise_2D(float x, float y);
};
#endif