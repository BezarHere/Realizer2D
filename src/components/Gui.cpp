#include "global.h"
#include "Gui.h"
#include "servers/ResourcesServer.h"

_R2D_COMP_NS_START_
constexpr uint32_t DefaultFontSize{ 12U };

//void GraphicalUI::draw(sf::RenderTarget& target, sf::RenderStates state) const
//{
//	target.draw(sf::RectangleShape({ 256.0f, 512.0f }), state);
//}

void GraphicalUI::setPosition(const Vector2& position)
{
	sf::Transformable::setPosition(position);
	m_rect.position(position);
}

void GraphicalUI::setPosition(real_t x, real_t y)
{
	sf::Transformable::setPosition(x, y);
	m_rect.position(x, y);
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
	m_rect.position(offset.x + m_rect.left, offset.y + m_rect.top);
}

void GraphicalUI::move(real_t x, real_t y)
{
	sf::Transformable::move(x, y);
	m_rect.position(x + m_rect.left, y + m_rect.top);
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

Text::Text()
	: m_inner{"", ResourcesServer::GetDefaultFont(), DefaultFontSize }
{
}

Text::Text(const std::string& text)
	: m_inner{ text, ResourcesServer::GetDefaultFont(), DefaultFontSize }
{
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	state.transform.combine(getTransform());
	target.draw(m_inner, state);
}

sf::Text& Text::getInner()
{
	return m_inner;
}

const sf::Text& Text::getInner() const
{
	return m_inner;
}

void Text::setText(const std::string& text)
{
	m_inner.setString(text);
}

_R2D_COMP_NS_END_