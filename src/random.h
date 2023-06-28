#pragma once
#include <random>
#include <chrono>
#include "global.h"

_R2D_NAMESPACE_START_

namespace random
{

	inline void randomize()
	{
		srand((uint32_t)(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
	}

	// [0.0f, 1.0f]
	inline float rand_float()
	{
		return (float)rand() / 32767.0f;
	}

	// [0.0, 1.0]
	inline double rand_double()
	{
		return (double)rand() / 32767.0;
	}

	inline int8_t rand_8u()
	{
		return (uint8_t)(rand() & 0xff);
	}

	inline int8_t rand_8()
	{
		return (int8_t)(rand() & 0xff);
	}

	inline int16_t rand_16()
	{
		return (int16_t)rand();
	}

	inline int32_t rand_32()
	{
		return (rand() << 16) + rand();
	}

	inline int64_t rand_64()
	{
		return ((int64_t)(rand_32()) << 32) + rand_32();
	}

	inline float randf_range(float from, float to)
	{
		if (from > to)
			doswap(from, to);
		return from + (rand_float() * (to - from));
	}

	inline double randd_range(double from, double to)
	{
		if (from > to)
			doswap(from, to);
		return from + (rand_double() * (to - from));
	}

	inline int32_t randi_range(int32_t from, int32_t to)
	{
		if (from > to)
			doswap(from, to);
		return from + (rand_32() % (to - from));
	}

}

_R2D_NAMESPACE_END_