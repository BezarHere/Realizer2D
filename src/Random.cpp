#include "global.h"
#include "Random.h"

_R2D_NAMESPACE_START_

RandomInstance::RandomInstance(uint32_t seed)
	: m_seed(seed), m_state(seed)
{
}

void RandomInstance::setSeed(uint32_t seed)
{
	m_seed = seed;
}

uint32_t RandomInstance::getSeed() const
{
	return m_seed;
}
uint32_t RandomInstance::getState() const
{
	return m_state;
}

void RandomInstance::advanceState()
{
	m_state = (uint32_t)((r2d::combine_hash(m_seed, m_state) >> 16) & 0xffffffff);
}

void Random::Randomize()
{
	srand((uint32_t)(std::chrono::high_resolution_clock::now().time_since_epoch().count() & 0xffffffff));
}

float Random::RandFloat()
{
	return (float)rand() / 32767.0f;
}

double Random::RandDouble()
{
	return (double)rand() / 32767.0;
}

int8_t Random::Rand8U()
{
	return (uint8_t)(rand() & 0xff);
}

int8_t Random::Rand8()
{
	return (int8_t)(rand() & 0xff);
}

int16_t Random::Rand16()
{
	return (int16_t)rand();
}

int32_t Random::Rand32()
{
	return (rand() << 16) + rand();
}

int64_t Random::Rand64()
{
	return ((int64_t)(Rand32()) << 32) + Rand32();
}

float Random::RandfRange(float from, float to)
{
	if (from > to)
		doswap(from, to);
	return from + (RandFloat() * (to - from));
}

double Random::RanddRange(double from, double to)
{
	if (from > to)
		doswap(from, to);
	return from + (RandDouble() * (to - from));
}

int32_t Random::RandiRange(int32_t from, int32_t to)
{
	if (from > to)
		doswap(from, to);
	return from + (Rand32() % (to - from));
}


_R2D_NAMESPACE_END_

