#include "global.h"
#include "Shape.h"

R2DN
{



float Shape::hasPoint(sf::Vector2f point) const
{
  return 0.0f;
}

const Points_t& Shape::getNormals() const
{
    const Points_t& points = getPoints();
    const size_t points_count = points.size();
    Points_t normals;
    if (points_count < 3)
    {
      throw std::runtime_error("default getNormals() implemention can't support shapes with points < 3");
      return normals;
    }
    normals.reserve(points.size());

    // will break for any concave shapes
    bool clockwise_winding = (points[2] - points[0]).dot(points[0].tangent()) < 0.0f;


    // i know this is shitty
    if (clockwise_winding)
    {
      for (size_t i{ 0 }; i < points.size(); i++)
      {
        normals.push_back(sf::Vector2f());
      }
    }
    else
    {
      for (size_t i{ 0 }; i < points.size(); i++)
      {
        normals.push_back(sf::Vector2f());
      }
    }

    return normals;
}

}
