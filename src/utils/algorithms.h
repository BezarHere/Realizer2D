#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

extern Vector2 averagePoint(const Points_t& points);
extern Vector2 furthestPoint(const Points_t& points, const Vector2& direction);
extern Vector2 getSupport(const Points_t& points_a, const Points_t& points_b, const Vector2& direction);
extern Vector2 tripleProduct(const Vector2& a, const Vector2& b, const Vector2& c);
extern bool GJKTest(const Points_t &points_a, const Points_t &points_b);

template <typename _T>
constexpr _T lerp(_T value, const _T min_value, const _T max_value)
{
	return (value - min_value) / (max_value - min_value);
}

_R2D_NAMESPACE_END_