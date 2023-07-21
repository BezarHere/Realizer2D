#pragma once
#include "base.h"

_R2D_COMP_NS_START_

class GraphicalUI : public ObjectComponent, public sf::Transformable
{
public:

	//void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	void setPosition(const Vector2 &position);
	void setPosition(real_t x, real_t y);
	void setOrigin(const Vector2 &origin);
	void setOrigin(real_t x, real_t y);
	void move(const Vector2 &offset);
	void move(real_t x, real_t y);
	void scale(const Vector2 &factor);
	void scale(real_t x, real_t y);
	void rotate(real_t angle);


	void setScale(const Vector2& scale);
	void setScale(real_t x, real_t y);

private:
	Rectf m_rect{};
};

class Text : public GraphicalUI
{
public:
	Text();
	Text(const std::string& text);

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	sf::Text& getInner();
	const sf::Text& getInner() const;
	
	void setText(const std::string& text);

private:
	sf::Text m_inner;
};

_R2D_COMP_NS_END_