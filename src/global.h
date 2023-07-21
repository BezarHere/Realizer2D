#pragma once

#define R2D r2d
#define _R2D_ R2D::
#define R2DN namespace R2D
#define _R2D_NAMESPACE_START_ namespace R2D {
#define _R2D_NAMESPACE_END_ }
#define SFML_STATIC

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <assert.h>
#include <unordered_map>

#include "../include/prefix.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// unrefrenced formal function paramter
#pragma warning(disable : 4100)

_R2D_NAMESPACE_START_

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif // !max

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif // !min

#ifndef doswap
#define doswap(a, b) __doswap__(a, b)
#endif // !swap_ab

#ifdef HIGH_PRECI
typedef double real_t;
#else
typedef float real_t;
#endif // HIGH_PRECI

template <typename _T>
inline constexpr size_t lengthof(const _T* const x)
{
	return ((size_t)sizeof(x)) / ((size_t)sizeof(*x));
}

typedef sf::FloatRect Rectf;
typedef sf::IntRect Recti;

typedef sf::Vector2<real_t> Vector2;
typedef sf::Vector2<int> Point2;
typedef sf::Vector2<uint32_t> Size2;
typedef sf::Color Color;
typedef sf::Transform Transform2D;
typedef sf::Font Font;

typedef void (*Action_t)();
typedef void (*DrawerFunction_t)(sf::RenderTarget& target, sf::RenderStates state);
typedef Action_t ProcessFunction_t;
typedef void (*PhysicsFunction_t)(float delta);

typedef std::vector<Vector2> Points_t;

template <class T>
inline void __doswap__(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

#define _r2d_error(msg) std::cerr << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#define _r2d_warning(msg) std::clog << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#ifdef _DEBUG
#define _r2d_print(msg) std::cout << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#elif
#define _r2d_print(msg)
#endif

//extern void _r2d_error(std::string l);
//extern void _r2d_warning(std::string l);
// 
// use as a predicate in 'std::find'
template <typename _T>
struct _Find_Value_In_Map
{
	template <typename _TK>
	inline bool operator()(const std::pair<_TK, _T>& pair) const
	{
		return pair.second == value;
	}

	_T value;
};

#define deg2rad(deg) (deg * (r2d::Pi / r2d::real_t(180.0)))
#define rad2deg(rad) (rad * (r2d::real_t(180.0) / r2d::Pi))

// if cond is false, the msg will be printed and the program will halt
#define assert_msg(cond, msg) if (!(cond)) { std::cerr << "R2D-ERROR: " << __FILE__ << ':' << __LINE__ << "\n  " << (msg) << std::endl; throw std::runtime_error(#cond); }

template <class T>
inline consteval const char* this_name(T* value) { return typeid(*value).raw_name(); }

inline std::ostream& operator<<(std::ostream & stream, const sf::Color & p) {
	stream << '(' << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ", " << (int)p.a << ')';
	return stream;
}



extern std::string MyPath;


enum : uint8_t {
	Clockwise,
	CounterClockwise
};

constexpr real_t Pi = (real_t)3.141592653589793;
constexpr real_t Tau = Pi * (real_t)2.0;
constexpr real_t E = (real_t)2.718281828459045;
constexpr real_t Eular = E;



#define PEEK(p) std::cout << #p << ": " << (p) << '\n'


template <class T>
sf::Vector2<T> operator/(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
	return sf::Vector2<T>(left.x / right.x, left.y / right.y);
}

template <class T>
sf::Vector2<T> operator/(const T& left, const sf::Vector2<T>& right)
{
	return sf::Vector2<T>(left / right.x, left / right.y);
}


// the intersection between the to rects
template <class T> inline sf::Rect<T> operator&(const sf::Rect<T>& left, const sf::Rect<T>& right)
{
	const sf::Vector2<T> position = sf::Vector2<T>(max(left.left, right.left), max(left.top, right.top));
	return sf::Rect<T>(
		position.x, position.y,
		(left.left + left.width < right.left + right.width ? left.left + left.width : right.left + right.width) - position.x,
		(left.top + left.height < right.top + right.height ? left.top + left.height : right.top + right.height) - position.y);
}

// end of the rect
template <class T> inline sf::Vector2<T> operator~(const sf::Rect<T>& p)
{
	return sf::Vector2<T>(p.left + p.width, p.top + p.height);
}


// a bigger rect overlaping the two rects
template <class T> inline sf::Rect<T> operator|(const sf::Rect<T>& left, const sf::Rect<T>& right)
{
	const sf::Vector2<T> position = sf::Vector2<T>(min(left.left, right.left), min(left.top, right.top));
	return sf::Rect<T>(
		position.x, position.y,
		(left.left + left.width > right.left + right.width ? left.left + left.width : right.left + right.width) - position.x,
		(left.top + left.height > right.top + right.height ? left.top + left.height : right.top + right.height) - position.y);
}

inline constexpr size_t combine_hash(size_t lhs, size_t rhs) {
	lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
	return lhs;
}


_R2D_NAMESPACE_END_

namespace std
{
	template <class T>
	inline string to_string(sf::Vector2<T> vec) {
		return "(" + to_string(vec.x) + ", " + to_string(vec.y) + ")";
	}
}
