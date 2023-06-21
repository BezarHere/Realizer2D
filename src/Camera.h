#pragma once
#include "Object2D.h"

R2DN
{

	class Camera : public Object2D
	{
	public:
		sf::Vector2f getZoom() const { return getScale(); }

	private:


	private:
		bool active{ false };
	};

}
