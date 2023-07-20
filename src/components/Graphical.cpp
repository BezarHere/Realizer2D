#include "global.h"
#include "Graphical.h"

namespace R2D::components
{
	Drawer::Drawer()
		: m_mat()
	{
	}

	Material& Drawer::getMaterial()
	{
		return m_mat;
	}

	const Material& Drawer::getMaterial() const
	{
		return m_mat;
	}

	void Drawer::setMaterial(const Material& mat)
	{
		//m_mat = mat;
	}

	RectangleDrawer::RectangleDrawer()
		: RectangleDrawer({ 16.0f, 16.0f })
	{
	}

	RectangleDrawer::RectangleDrawer(Vector2 size)
		: m_size(size)
	{
		update_buffer();
	}

	RectangleDrawer::~RectangleDrawer()
	{
	}

	void RectangleDrawer::setSize(Vector2 size)
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
		target.draw(m_buffer.getBuffer(), state);
	}

	void RectangleDrawer::update_buffer()
	{
		sf::Vertex vertcies[]
		{
			{ Vector2(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ Vector2(m_size.x / 2.0f, -m_size.y / 2.0f), color },
			{ Vector2(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2(m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2(-m_size.x / 2.0f, m_size.y / 2.0f), color },
			{ Vector2(-m_size.x / 2.0f, -m_size.y / 2.0f), color },
		};
		m_buffer.getBuffer().update(vertcies);
	}

	CircleDrawer::CircleDrawer()
		: CircleDrawer(8.0f, 16U)
	{
	}

	CircleDrawer::CircleDrawer(real_t radius, uint16_t segments_count)
		: m_radius(radius), m_segmentsCount(segments_count)
	{
		updateState();
	}

	CircleDrawer::~CircleDrawer()
	{
	}

	void CircleDrawer::setRadius(real_t radius)
	{
		m_radius = radius;
		updateState();
	}

	void components::CircleDrawer::setSegmentsCount(uint16_t segments_count)
	{
		if (segments_count == m_segmentsCount)
			return;
		m_segmentsCount = segments_count;
		updateState();
	}

	void CircleDrawer::draw(sf::RenderTarget& target, sf::RenderStates state) const
	{
		state.transform.combine(getTransform());
		target.draw(m_buffer.getBuffer(), state);
	}

	void CircleDrawer::updateState()
	{
		if (m_segmentsCount < 3)
		{
			m_buffer.getBuffer().create(0);
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

		m_buffer.getBuffer().create(poly_size);
		m_buffer.getBuffer().update(vertcies);
	}

	bool CircleDrawer::hasVertciesCacheSegCount(uint16_t seg_count)
	{
		return s_vertexCache.find(seg_count) != s_vertexCache.end();
	}

	void CircleDrawer::createVertciesCache(uint16_t seg_count)
	{
		assert_msg(seg_count >= 3, "can't create a cricle polygon with less then 3 verts")
			const Vector2 pivot {
			1.0f, 0.0f
		};
		real_t rot_step = Tau / (real_t)(seg_count);
		Vector2 rotator = pivot.rotated(rot_step);
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

	SpriteDrawer::SpriteDrawer()
	{
		updateVertcies();
	}

	SpriteDrawer::SpriteDrawer(sf::Texture* texture)
		: m_texture(texture)
	{
		updateVertcies();
	}

	void SpriteDrawer::setTexture(sf::Texture* texture)
	{
		m_texture = texture;
		updateVertcies();
	}

	void SpriteDrawer::setFlipV(bool flipv)
	{
		if (flipv == m_flipV)
			return;
		m_flipV = flipv;
		updateVertcies();
	}

	void SpriteDrawer::setFlipH(bool fliph)
	{
		if (fliph == m_flipH)
			return;
		m_flipH = fliph;
		updateVertcies();
	}

	void SpriteDrawer::setStretchMode(SpriteStretchMode mode)
	{
		m_stretchMode = mode;
		updateVertcies();
	}

	void SpriteDrawer::setSize(sf::Vector2f size)
	{
		if (size == m_size)
			return;
		m_size = size;
		updateVertcies();
	}

	void SpriteDrawer::updateTexture(sf::Texture* texture)
	{
		m_texture = texture;
	}

	void SpriteDrawer::draw(sf::RenderTarget& target, sf::RenderStates state) const
	{
		if (m_flipH || m_flipV)
		{
			state.transform.scale(sf::Vector2f(m_flipH ? -1.0f : 1.0f, m_flipV ? -1.0f : 1.0f));
		}
		state.transform.combine(getTransform());
		state.texture = m_texture;
		target.draw(m_buffer.getBuffer(), state);
	}

	void SpriteDrawer::updateVertcies()
	{
		if (!m_texture)
			return;

		sf::Vector2f draw_size
		{
			m_stretchMode == SpriteStretchMode::Expand ? (sf::Vector2f)m_texture->getSize() : m_size
		};
		sf::Vector2f half_draw_size{ draw_size / 2.0f };

		sf::Vertex vertcies[6]
		{
			sf::Vertex(-half_draw_size, color, {0.0f, 0.0f}),
			sf::Vertex({half_draw_size.x, -half_draw_size.y}, color, {draw_size.x, 0.0f}),
			sf::Vertex(half_draw_size, color, draw_size),

			sf::Vertex(half_draw_size, color, draw_size),
			sf::Vertex({-half_draw_size.x, half_draw_size.y}, color, {0.0f, draw_size.y}),
			sf::Vertex(-half_draw_size, color, {0.0f, 0.0f}),
		};
		m_buffer.getBuffer().update(vertcies);
	}

	PolygonDrawer::PolygonDrawer()
	{
		updateVertcies();
	}

	PolygonDrawer::PolygonDrawer(Points_t points)
		: m_points{points}
	{
		updateVertcies();
	}

	PolygonDrawer::~PolygonDrawer()
	{
	}

	void PolygonDrawer::draw(sf::RenderTarget& target, sf::RenderStates state) const
	{
		state.transform.combine(getTransform());
		target.draw(m_buffer.getBuffer(), state);
	}

	void PolygonDrawer::setPoints(const Points_t& points)
	{
		m_points = points;
		updateVertcies();
	}

	void PolygonDrawer::updateVertcies()
	{
		size_t points_count = m_points.size();
		if (points_count < 3)
		{
			m_buffer.getBuffer().create(0);
			return;
		}

		size_t vertcies_count = (points_count - 2) * 3;
		sf::Vertex* vertcies = new sf::Vertex[vertcies_count];

		for (size_t i{ 0 }; i < (points_count - 2); i++)
		{
			size_t i3 = i * 3;
			vertcies[i3].position = m_points[0];
			vertcies[i3].color = color;
			vertcies[i3 + 1].position = m_points[i + 1];
			vertcies[i3 + 1].color = color;
			vertcies[i3 + 2].position = m_points[i + 2];
			vertcies[i3 + 2].color = color;
		}
		
		m_buffer.getBuffer().create(vertcies_count);
		m_buffer.getBuffer().update(vertcies);

		delete[] vertcies;
	}

}

