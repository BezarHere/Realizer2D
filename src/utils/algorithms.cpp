#include "global.h"
#include "algorithms.h"

_R2D_NAMESPACE_START_

Vector2 averagePoint(const Points_t& points)
{
  Vector2 base;
  for (const Vector2& p : points) base += p;
  return  base / (real_t)points.size();
}

Vector2 furthestPoint(const Points_t& points, const Vector2 &direction)
{
  real_t maxProduct = direction.dot(points[0]);
  Vector2 value{};
  for (const Vector2& p : points) {
    real_t product = direction.dot(p);
    if (product > maxProduct) {
      maxProduct = product;
      value = p;
    }
  }
  return value;
}

Vector2 getSupport(const Points_t& points_a, const Points_t& points_b, const Vector2 &direction)
{
  // get furthest point of first body along an arbitrary direction
  //size_t i = indexOfFurthestPoint(vertices1, count1, d);

  // get furthest point of second body along the opposite direction
  //size_t j = indexOfFurthestPoint(vertices2, count2, negate(d));

  // subtract (Minkowski sum) the two points to see if bodies 'overlap'
  //return subtract(vertices1[i], vertices2[j]);
  return furthestPoint(points_a, direction) - furthestPoint(points_b, -direction);
}

Vector2 tripleProduct(const Vector2& a, const Vector2& b, const Vector2& c)
{
  real_t adotc = a.x * c.x + a.y * c.y; // perform a.dot(c)
  real_t bdotc = b.x * c.x + b.y * c.y; // perform b.dot(c)
  // perform b * a.dot(c) - a * b.dot(c)
  return { b.x * adotc - a.x * bdotc, b.y * adotc - a.y * bdotc };
}


bool GJKTest(const Points_t& points_a, const Points_t& points_b)
{
  size_t index = 0; // index of current vertex of simplex
  Vector2 a, b, c, d, ao, ab, ac, abperp, acperp, simplex[3];

  Vector2 position1 = averagePoint(points_a); // not a CoG but
  Vector2 position2 = averagePoint(points_b); // it's ok for GJK )

  // initial direction from the center of 1st body to the center of 2nd body
  d = position1 - position2;

  // if initial direction is zero – set it to any arbitrary axis (we choose X)
  if ((d.x == 0) && (d.y == 0))
    d.x = 1.f;

  // set the first support as initial point of the new simplex
  a = simplex[0] = getSupport(points_a, points_b, d);

  if (a.dot(d) <= 0)
  {
    return false;
  }

  d = -a; // The next search direction is always towards the origin, so the next search direction is negate(a)

  while (1) {

    a = simplex[++index] = getSupport(points_a, points_b, d);

    if (a.dot(d) <= 0)
    {
      PEEK(a);
      PEEK(index);
      // no collision
      return false;
    }

    ao = -a; // from point A to Origin is just negative A

    // simplex has 2 points (a line segment, not a triangle yet) []
    if (index < 2) {
      b = simplex[0];
      ab = b - a; // from point A to B
      d = tripleProduct(ab, ao, ab); // normal to AB towards Origin
      if (d.lengthSquared() == 0)
        d = ab.tangent();
      continue; // skip to next iteration
    }

    b = simplex[1];
    c = simplex[0];
    ab = b - a; // from point A to B
    ac = c - a; // from point A to C

    acperp = tripleProduct(ab, ac, ac);

    if (acperp.dot(ao) >= 0) {

      d = acperp; // new direction is normal to AC towards Origin

    }
    else {

      abperp = tripleProduct(ac, ab, ab);

      if (abperp.dot(ao) < 0)
        return true; // collision

      simplex[0] = simplex[1]; // swap first element (point C)

      d = abperp; // new direction is normal to AB towards Origin
    }

    simplex[1] = simplex[2]; // swap element in the middle (point B)
    --index;
  }
  _r2d_print("fail 3");
  return false;
}

_R2D_NAMESPACE_END_