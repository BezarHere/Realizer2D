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
		RectangleDrawer(Vector2f_t size);
		~RectangleDrawer();

		void setSize(Vector2f_t size);
		inline Vector2f_t getSize() const { return m_size; }
		void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
		inline void setColor(sf::Color clr) override { color = clr; update_buffer(); }
	private:
		void update_buffer();
	private:
		Vector2f_t m_size{ 16.0f, 16.0f };
		utils::VertexBufferWraper m_buffer{ sf::Triangles, sf::VertexBuffer::Dynamic, 6 };
	};

	class CircleDrawer : public Drawer
	{
	public:
		CircleDrawer();
		CircleDrawer(real_t radius, uint16_t segments_count = 16);
		~CircleDrawer();

		void setRadius(real_t radius);
		void setSegmentsCount(uint16_t segments_count);
		real_t getRadius() const { return m_radius; }
		uint16_t getSegmentsCount() const { return m_segmentsCount; }
		void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
		inline void setColor(sf::Color clr) override { color = clr; update_buffer(); }
	private:
		void update_buffer();
		static bool hasVertciesCacheSegCount(uint16_t seg_count);
		static void createVertciesCache(uint16_t seg_count);
		static const Points_t& getVertciesFromCache(uint16_t seg_count);
	private:
		real_t m_radius = 16.0f;
		uint16_t m_segmentsCount = 16;
		utils::VertexBufferWraper m_buffer{ sf::Triangles, sf::VertexBuffer::Dynamic };
		static inline std::unordered_map<uint16_t, Points_t> s_vertexCache;
	};

}
_R2D_NAMESPACE_END_
