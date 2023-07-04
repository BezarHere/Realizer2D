#include "global.h"
#include "Drawers.h"

namespace R2D::components
{
	RectangleDrawer::RectangleDrawer()
		: RectangleDrawer({ 16.0f, 16.0f })
	{
	}

	RectangleDrawer::RectangleDrawer(Vector2f_t size)
		: m_size(size)
	{
		update_buffer();
	}

	RectangleDrawer::~RectangleDrawer()
	{
	}

	void RectangleDrawer::setSize(Vector2f_t size)
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
			{ Vector2f_t(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ Vector2f_t(m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ Vector2f_t(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2f_t(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2f_t(-m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2f_t(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
		};
		m_buffer.getBuffer()->update(vertcies);
	}


	CircleDrawer::CircleDrawer()
		: CircleDrawer(8.0f, 16U)
	{
	}

	CircleDrawer::CircleDrawer(real_t radius, uint16_t segments_count)
		: m_radius(radius), m_segmentsCount(segments_count)
	{
		update_buffer();
	}

	CircleDrawer::~CircleDrawer()
	{
	}

	void CircleDrawer::setRadius(real_t radius)
	{
		m_radius = radius;
		update_buffer();
	}

	void components::CircleDrawer::setSegmentsCount(uint16_t segments_count)
	{
		if (segments_count == m_segmentsCount)
			return;
		m_segmentsCount = segments_count;
		update_buffer();
	}

	void CircleDrawer::draw(sf::RenderTarget& target, sf::RenderStates state) const
	{
		state.transform.combine(getTransform());
		target.draw(*(m_buffer.getBuffer()), state);
	}

	void CircleDrawer::update_buffer()
	{
		if (m_segmentsCount < 3)
		{
			m_buffer.getBuffer()->create(0);
			return;
		}

		// way better the recalculating the entire polygon
		// not using sin & cos (atleast not every time this is called) is better

		const Points_t& base_poly = getVertciesFromCache(m_segmentsCount);
		const size_t poly_size = base_poly.size();
		sf::Vertex* vertcies = new sf::Vertex[poly_size];
		const sf::Color clr = getColor();

		for (uint16_t i{ 0U }; i < poly_size; i++)
		{
			vertcies[i].position = base_poly[i] * m_radius;
			vertcies[i].color = clr;
		}
		
		m_buffer.getBuffer()->create(poly_size);
		m_buffer.getBuffer()->update(vertcies);
	}

	bool CircleDrawer::hasVertciesCacheSegCount(uint16_t seg_count)
	{
		return s_vertexCache.find(seg_count) != s_vertexCache.end();
	}

	void CircleDrawer::createVertciesCache(uint16_t seg_count)
	{
		assert_msg(seg_count >= 3, "can't create a cricle polygon with less then 3 verts")
		const Vector2f_t pivot { 1.0f, 0.0f };
		real_t rot_step = Tau / (real_t)(seg_count);
		Vector2f_t rotator = pivot.rotated(rot_step);
		Points_t vertices;


		for (uint16_t i{ 0U }; i < seg_count - 2; i++)
		{
			vertices.push_back(pivot);
			vertices.push_back(rotator);
			rotator.rotate(rot_step);
			vertices.push_back(rotator);
		}
		s_vertexCache.insert_or_assign(seg_count, vertices);
	}

	const Points_t& CircleDrawer::getVertciesFromCache(uint16_t seg_count)
	{
		if (!hasVertciesCacheSegCount(seg_count))
		{
			createVertciesCache(seg_count);
		}
		return s_vertexCache.at(seg_count);
	}

}

