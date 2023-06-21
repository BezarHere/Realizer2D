#pragma once

#define R2D r2d
#define _R2D_ R2D::
#define R2DN namespace R2D
#define SFML_STATIC

#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

namespace R2D
{
	typedef std::vector<sf::Vector2f> Points_t;

	extern std::string MyPath;
	extern sf::RenderWindow* MainWindow;


}

