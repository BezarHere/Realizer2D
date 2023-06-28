#include "global.h"
#include "Drawers.h"

namespace R2D::components
{
	RectangleDrawer::RectangleDrawer()
		: RectangleDrawer({ 16.0f, 16.0f })
	{
	}

	RectangleDrawer::RectangleDrawer(sf::Vector2f size)
		: m_size(size)
	{
		update_buffer();
	}

	RectangleDrawer::~RectangleDrawer()
	{
	}

	void RectangleDrawer::setSize(sf::Vector2f size)
	{
		if (size == m_size)
		{
			return;
		}
		m_size = size;
		update_buffer();
	}

	void RectangleDrawer::draw(sf::RenderTarget& target, sf::RenderStates state) const
	{
		state.transform.combine(getTransform());
		target.draw(*(m_buffer.getBuffer()), state);
	}

	void RectangleDrawer::update_buffer()
	{
		sf::Vertex vertcies[]
		{
			{ sf::Vector2f(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ sf::Vector2f(m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ sf::Vector2f(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ sf::Vector2f(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ sf::Vector2f(-m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ sf::Vector2f(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
		};
		m_buffer.getBuffer()->update(vertcies);
	}

}
