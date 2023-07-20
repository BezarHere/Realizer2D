#include "global.h"
#include "Gui.h"

_R2D_COMP_NS_START_

void GraphicalUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(sf::RectangleShape({ 256.0f, 512.0f }), state);
}

void GraphicalUI::setPosition(const Vector2& position)
{
	sf::Transformable::setPosition(position);
	m_rect.setPosition(position);
}

void GraphicalUI::setPosition(real_t x, real_t y)
{
	sf::Transformable::setPosition(x, y);
	m_rect.setPosition(x, y);
}

// TODO: scales?
void GraphicalUI::setScale(const Vector2& scale)
{
	sf::Transformable::setScale(scale);
}

void GraphicalUI::setScale(real_t x, real_t y)
{
	sf::Transformable::setScale(x, y);
}

void GraphicalUI::setOrigin(const Vector2& origin)
{
	sf::Transformable::setOrigin(origin);
}

void GraphicalUI::setOrigin(real_t x, real_t y)
{
	sf::Transformable::setOrigin(x, y);
}

void GraphicalUI::move(const Vector2& offset)
{
	sf::Transformable::move(offset);
	m_rect.setPosition(offset.x + m_rect.left, offset.y + m_rect.top);
}

void GraphicalUI::move(real_t x, real_t y)
{
	sf::Transformable::move(x, y);
	m_rect.setPosition(x + m_rect.left, y + m_rect.top);
}

void GraphicalUI::scale(const Vector2& factor)
{
	sf::Transformable::scale(factor);
}

void GraphicalUI::scale(real_t x, real_t y)
{
	sf::Transformable::scale(x, y);
}

void GraphicalUI::rotate(real_t angle)
{
	sf::Transformable::rotate(angle);
}

_R2D_COMP_NS_END_