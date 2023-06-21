#pragma once

R2DN
{

/// Shapes like circles, squares, tringles and convex polygons
/// no built-in support for concave polygons for now
/// if any implemention of you derived shape has a concave polygon, then get ready for bugs
class Shape
{
public:
	virtual float getArea() const = 0;
	virtual const Points_t& getPoints() const = 0;

	// checks if the point is inside the shape
	virtual float hasPoint(sf::Vector2f point) const;

	// every normal at index n represents the normal for segment fron index n to index n + 1 % N
	// will not work with concave shapes
	virtual const Points_t& getNormals() const;

};

}

