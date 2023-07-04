////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_VECTOR2_HPP
#define SFML_VECTOR2_HPP


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility template class for manipulating
///        2-dimensional vectors
///
////////////////////////////////////////////////////////////
template <typename T>
class Vector2
{
public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		/// Creates a Vector2(0, 0).
		///
		////////////////////////////////////////////////////////////
		Vector2();

		////////////////////////////////////////////////////////////
		/// \brief Construct the vector from its coordinates
		///
		/// \param X X coordinate
		/// \param Y Y coordinate
		///
		////////////////////////////////////////////////////////////
		Vector2(T X, T Y);

		////////////////////////////////////////////////////////////
		/// \brief Construct the vector from another type of vector
		///
		/// This constructor doesn't replace the copy constructor,
		/// it's called only when U != T.
		/// A call to this constructor will fail to compile if U
		/// is not convertible to T.
		///
		/// \param vector Vector to convert
		///
		////////////////////////////////////////////////////////////
		template <typename U>
		explicit Vector2(const Vector2<U>& vector);
		
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
		/// Next section is made by bezar
		////////////////////////////////////////////////////////////

		inline float length() const
		{
			return std::sqrt((x * x) + (y * y));
		}

		inline float lengthSquared() const
		{
			return (x * x) + (y * y);
		}

		inline float distance(const Vector2<T>& other) const
		{
			T a = x - other.x;
			T b = y - other.y;
			return std::sqrt((a * a) + (b * b));
		}

		inline float distanceSquared(const Vector2<T>& other) const
		{
			T a = x - other.x;
			T b = y - other.y;
			return (a * a) + (b * b);
		}

		inline Vector2<T> normalized() const
		{
			float l = length();
			if (l < 1.0e-5f)
				return Vector2<T>((T)0, (T)0);
			return Vector2<T>(x, y) / l;
		}

		inline Vector2<T> tangent() const
		{
			return Vector2<T>(-y, x);
		}

		inline Vector2<T> normalTangent() const
		{
			float l = length();
			if (l < 1.0e-5f)
				return Vector2<T>((T)0, (T)0);
			return Vector2<T>(-y / l, x / l);
		}

		inline float dot(Vector2<T> other) const
		{
			return (x * other.x) + (y * other.y);
		}

		inline float cross(Vector2<T> other) const
		{
			return (x * other.x) - (y * other.y);
		}

		inline Vector2<T> linearInterpolate(Vector2<T> other, float dt) const
		{
			return linearInterpolate(*this, other, dt);
		}

		inline Vector2<T> quadraticInterpolate(Vector2<T> other, float dt) const
		{
			return quadraticInterpolate(*this, other, dt);
		}

		inline Vector2<T> cubicInterpolate(Vector2<T> other, float dt) const
		{
			return quadraticInterpolate(*this, other, dt);
		}

		// in radians
		inline float angle() const
		{
			return std::atan2(y, x);
		}

		// in radians
		inline float angleTo(const Vector2<T>& other) const
		{
			return std::atan2(y - other.y, x - other.x);
		}

		inline Vector2<T> rotated(float angle_radians) const
		{
			float ang = angle() + angle_radians;
			float len = length();
			return Vector2<T>(std::cos(ang) * len, std::sin(ang) * len);
		}

		// [modifier]
		inline void rotate(float angle_radians)
		{
			float ang = angle() + angle_radians;
			float len = length();
			x = std::cos(ang) * len;
			y = std::sin(ang) * len;
		}

		template <class E>
		inline static Vector2<E> linearInterpolate(const Vector2<E>& a, const Vector2<E>& b, float dt)
		{
			
			return a + ((b - a) * dt);
		}

		template <class E>
		inline static Vector2<E> quadraticInterpolate(const Vector2<E>& a, const Vector2<E>& b, float dt)
		{
			dt *= dt;
			return a + ((b - a) * dt);
		}

		template <class E>
		inline static Vector2<E> cubicInterpolate(const Vector2<E>& a, const Vector2<E>& b, float dt)
		{
			dt *= dt * dt;
			return a + ((b - a) * dt);
		}

		// angle '0' is a vector pointing to the left with angle 90 pointing to up
		inline static Vector2<T> polar2coord(float angle_rad, float len)
		{
			return Vector2<T>(std::cos(angle_rad) * len, std::sin(angle_rad) * len);
		}

		// first is the *angle* and second is the length
		inline std::pair<float, float> coord2polar()
		{
			return { angle(), length() };
		}
	

		template <typename T, typename E>
		inline sf::Vector2<T> operator *(const Vector2<E>& other)
		{
			return sf::Vector2<T>(x * other.x, y * other.y);
		}

		////////////////////////////////////////////////////////////
		/// end of the modified section
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		T x; ///< X coordinate of the vector
		T y; ///< Y coordinate of the vector
};

template <typename T> struct std::hash<sf::Vector2<T>> {
inline size_t operator()(const sf::Vector2<T>& obj) const {
		return combine_hash(std::hash<T>()(obj.x), std::hash<T>()(obj.y));
	}
};

template <typename T> inline std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& p) {
	stream << '(' << p.x << ", " << p.y << ')';
	return stream;
}

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of unary operator -
///
/// \param right Vector to negate
///
/// \return Memberwise opposite of the vector
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator +=
///
/// This operator performs a memberwise addition of both vectors,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator -=
///
/// This operator performs a memberwise subtraction of both vectors,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator +
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise addition of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator -
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise subtraction of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise multiplication by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(const Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a scalar value)
/// \param right Right operand (a vector)
///
/// \return Memberwise multiplication by \a left
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(T left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator *=
///
/// This operator performs a memberwise multiplication by \a right,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator /
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise division by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator /(const Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator /=
///
/// This operator performs a memberwise division by \a right,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator /=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator ==(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator !=(const Vector2<T>& left, const Vector2<T>& right);

#include <SFML/System/Vector2.inl>

// Define the most common types
typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;

} // namespace sf


#endif // SFML_VECTOR2_HPP


////////////////////////////////////////////////////////////
/// \class sf::Vector2
/// \ingroup system
///
/// sf::Vector2 is a simple class that defines a mathematical
/// vector with two coordinates (x and y). It can be used to
/// represent anything that has two dimensions: a size, a point,
/// a velocity, etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// You generally don't have to care about the templated form (sf::Vector2<T>),
/// the most common specializations have special typedefs:
/// \li sf::Vector2<float> is sf::Vector2f
/// \li sf::Vector2<int> is sf::Vector2i
/// \li sf::Vector2<unsigned int> is sf::Vector2u
///
/// The sf::Vector2 class has a small and simple interface, its x and y members
/// can be accessed directly (there are no accessors like setX(), getX()) and it
/// contains mathematical function like dot product, cross product, length, etc.
///
/// Usage example:
/// \code
/// sf::Vector2f v1(16.5f, 24.f);
/// v1.x = 18.2f;
/// float y = v1.y;
///
/// sf::Vector2f v2 = v1 * 5.f;
/// sf::Vector2f v3;
/// v3 = v1 + v2;
///
/// bool different = (v2 != v3);
/// \endcode
///
/// Note: for 3-dimensional vectors, see sf::Vector3.
///
////////////////////////////////////////////////////////////
