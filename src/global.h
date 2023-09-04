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
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../include/prefix.h"

#include <Bite.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

// unrefrenced formal function paramter
#pragma warning(disable : 4100)

_R2D_NAMESPACE_START_

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // !MAX

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif // !MIN

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

// utils to make your terminal colorful (currently only works on windows)
namespace dye
{
	using namespace bite::dye;
}

using bite::StreamFrame;
using bite::StreamReader;
using bite::StreamWriter;
using bite::BufferSmartPtr_t;
using bite::BufferVector_t;

typedef sf::FloatRect Rectf;
typedef sf::IntRect Recti;

typedef sf::Vector2<real_t> Vector2;
typedef sf::Vector2<int> IVector2;
typedef sf::Vector2<uint32_t> Size2;
typedef sf::Color Color;
typedef sf::Transform Transform2D;
typedef sf::Font Font;

typedef void (*Action_t)();
typedef void (*DrawerFunction_t)(sf::RenderTarget& target, sf::RenderStates state);
typedef Action_t ProcessFunction_t;
typedef void (*PhysicsFunction_t)(float delta);

typedef std::vector<Vector2> Points_t;

typedef size_t hash_t;

template <class T>
inline void swapval(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

#define _r2d_error(msg) std::cerr << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#define _r2d_warning(msg) std::clog << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#ifdef _DEBUG
#define _r2d_print(msg) std::cout << "R2D: " << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl;
#else
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

inline constexpr real_t Pi = (real_t)3.141592653589793;
inline constexpr real_t Tau = Pi * (real_t)2.0;
inline constexpr real_t E = (real_t)2.718281828459045;
inline constexpr real_t Eular = E;
#ifndef R2D_PRECISE
inline constexpr real_t Epsilon = 1e-4f;
#else
inline constexpr real_t Epsilon = 1e-7f;
#endif


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

__forceinline constexpr size_t combine_hash(size_t lhs, size_t rhs) {
	return lhs ^ rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
}


template <typename T>
__forceinline constexpr size_t ContainerMemSpace(const T &c)
{
	return sizeof(c) + (sizeof(T::value_type) * c.capacity());
}

template <typename T>
__forceinline constexpr size_t ContainerMemUsage(const T &c)
{
	return sizeof(c) + (sizeof(T::value_type) * c.size());
}

template <typename _T>
__forceinline size_t &&hash(const _T &val)
{
	return std::hash<_T>()(
		std::move(val));
}

_R2D_NAMESPACE_END_

namespace std
{
	template <class T>
	inline string to_string(sf::Vector2<T> vec) {
		return "(" + to_string(vec.x) + ", " + to_string(vec.y) + ")";
	}

	template <typename _T, typename _E>
	struct hash<std::pair<_T, _E>>
	{
		size_t operator()(const std::pair<_T, _E> &p) const
		{
			return _R2D_ combine_hash(hash<_T>()(p.first), hash<_E>()(p.second));
		}
	};

	template <typename _Elm>
	struct hash<std::basic_string<_Elm>>
	{
		size_t operator()(const std::basic_string<_Elm> &_Keyval) const
		{
			size_t hash{};
			const char *const cstr = _Keyval.c_str();
			for (size_t i{}; i < _Keyval.length(); i++)
			{
				hash = _R2D_ combine_hash(hash, cstr[ i ]);
			}
			return hash;
		}

		size_t operator()(const std::basic_string<_Elm> &_Keyval, size_t len) const
		{
			size_t hash{};
			const char *const cstr = _Keyval.c_str();
			for (size_t i{}; i < len; i++)
			{
				hash = _R2D_ combine_hash(hash, cstr[ i ]);
			}
			return hash;
		}

	};

	template <>
	struct hash<char *>
	{

		size_t operator()(const char *_Keyval) const
		{
			size_t hash{};
			for (size_t i{}; _Keyval[ i ]; i++)
			{
				hash = _R2D_ combine_hash(hash, _Keyval[ i ]);
			}
			return hash;
		}

	};

	//template <typename _T>
	//struct hash<_T*>
	//{

	//	size_t operator()(const _T *_Keyval, size_t len) const
	//	{
	//		size_t hash{};
	//		for (size_t i{}; i < len; i++)
	//		{
	//			hash = _R2D_ combine_hash(hash, _Keyval[ i ]);
	//		}
	//		return hash;
	//	}

	//};
}
