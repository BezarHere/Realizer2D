#pragma once
#include <random>
#include <chrono>
#include "global.h"

_R2D_NAMESPACE_START_

class RandomInstance
{
public:
	RandomInstance(uint32_t seed);

	void setSeed(uint32_t seed);
	uint32_t getSeed() const;
	uint32_t getState() const;

private:
	void advanceState();

private:
	uint32_t m_seed;
	uint32_t m_state;
};

// TODO: make this reli on a shared RandomInstance object for random values
class Random
{
public:
	static void Randomize();
	// [0.0f, 1.0f]
	static float RandFloat();
	// [0.0, 1.0]
	static double RandDouble();
	// [0, 255]
	static int8_t Rand8U();
	static int8_t Rand8();
	static int16_t Rand16();
	static int32_t Rand32();
	static int64_t Rand64();
	static float RandfRange(float from, float to);
	static double RanddRange(double from, double to);
	static int32_t RandiRange(int32_t from, int32_t to);
private:
	static inline RandomInstance *s_sharedRandom = new RandomInstance(std::chrono::steady_clock::now().time_since_epoch().count() & 0xffffffff);
};

_R2D_NAMESPACE_END_