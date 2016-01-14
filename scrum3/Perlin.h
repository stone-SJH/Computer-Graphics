#ifndef _PERLIN_
#define _PERLIN_

#include <math.h>

class Perlin{
private:
	float persistence;
	int Number_Of_Octaves;

	void init();
	float Noise1(int x, int y);
	float SmoothNoise_1(int x, int y);
	float Cosine_Interpolate(float a, float b, float x);
	float InterpolatedNoise_1(float x, float y);

public:
	Perlin();
	float PerlinNoise_2D(float x, float y);
};
#endif