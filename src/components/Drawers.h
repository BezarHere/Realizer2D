#pragma once
#include "../ObjectComponent2D.h"
#include "../utils/VertexBufferWraper.h"

_R2D_NAMESPACE_START_

namespace components {
	class Drawer : public ObjectComponent2D, public sf::Transformable
	{
	public:

		inline virtual void setColor(sf::Color clr) { color = clr; }
		inline virtual const sf::Color& getColor() { return color; }

	protected:
		sf::Color color{ 255, 255, 255, 255 };
	};

	class RectangleDrawer : public Drawer
	{
	public:
		RectangleDrawer();
		RectangleDrawer(sf::Vector2f size);
		~RectangleDrawer();

		void setSize(sf::Vector2f size);
		void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
		inline void setColor(sf::Color clr) override { color = clr; update_buffer(); }
	private:
		void update_buffer();
	private:
		sf::Vector2f m_size{ 16.0f, 16.0f };
		utils::VertexBufferWraper m_buffer{ sf::Triangles, sf::VertexBuffer::Dynamic, 6 };
	};

}
_R2D_NAMESPACE_END_
