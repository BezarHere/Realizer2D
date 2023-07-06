#pragma once
#include "base.h"
#include "../utils/VertexBufferWraper.h"
#include "../core/StretchMode.h"

_R2D_COMP_NS_START_

constexpr uint32_t _RectangleDrawer_id		= 0xf421c19d;
constexpr uint32_t _Faker_id							= 0x9ac17fae;
constexpr uint32_t _CircleDrawer_id				= _RectangleDrawer_id ^ _Faker_id;
constexpr uint32_t _SpriteDrawer_id				= _CircleDrawer_id ^ _RectangleDrawer_id;
constexpr uint32_t _PolygonDrawer_id				= _CircleDrawer_id ^ _RectangleDrawer_id;

class Drawer : public ObjectComponent, public sf::Transformable
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
	RectangleDrawer(Vector2 size);
	~RectangleDrawer();

	void setSize(Vector2 size);
	inline Vector2 getSize() const { return m_size; }
	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	inline void setColor(sf::Color clr) override { color = clr; update_buffer(); }

	inline uint32_t getId() const { return _RectangleDrawer_id; }

private:
	void update_buffer();
private:
	Vector2 m_size{ 16.0f, 16.0f };
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
	inline void setColor(sf::Color clr) override { color = clr; updateState(); }

	inline uint32_t getId() const { return _CircleDrawer_id; }
private:
	void updateState();
	static bool hasVertciesCacheSegCount(uint16_t seg_count);
	static void createVertciesCache(uint16_t seg_count);
	static const Points_t& getVertciesFromCache(uint16_t seg_count);
private:
	real_t m_radius = 16.0f;
	uint16_t m_segmentsCount = 16;
	utils::VertexBufferWraper m_buffer{ sf::Triangles, sf::VertexBuffer::Dynamic };
	static inline std::unordered_map<uint16_t, Points_t> s_vertexCache;
};

class PolygonDrawer : public Drawer
{
public:
	PolygonDrawer();
	PolygonDrawer(Points_t points);
	~PolygonDrawer();

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
	void setPoints(const Points_t& points);
	const Points_t& getPoints() const { return m_points; }
	inline void setColor(sf::Color clr) override { color = clr; updateVertcies(); }

	inline uint32_t getId() const { return _PolygonDrawer_id; }
private:
	void updateVertcies();
private:
	Points_t m_points{};
	utils::VertexBufferWraper m_buffer{ sf::Triangles, sf::VertexBuffer::Dynamic };
};

class SpriteDrawer : public Drawer
{
public:
	SpriteDrawer();
	SpriteDrawer(sf::Texture *texture);

	inline const sf::Texture* getTexture() const { return m_texture; }
	inline bool getFlipV() const { return m_flipV; }
	inline bool getFlipH() const { return m_flipH; }
	inline SpriteStretchMode getStretchMode() const { return m_stretchMode; }
	inline sf::Vector2f getSize() const { return m_size; }

	void setTexture(sf::Texture* texture);
	void setFlipV(bool flipv);
	void setFlipH(bool fliph);
	void setStretchMode(SpriteStretchMode mode);
	void setSize(sf::Vector2f size);

	// lighter function then setTexture
	// this will not update the vertex buffer but will override the texture
	// so you need to update with a same size texture or this will break
	void updateTexture(sf::Texture* texture);

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	inline uint32_t getId() const { return _SpriteDrawer_id; }

	inline void setColor(sf::Color clr) override { color = clr; updateVertcies(); }
private:
	void updateVertcies();

private:
	SpriteStretchMode m_stretchMode{ SpriteStretchMode::Expand };
	sf::Vector2f m_size;
	sf::Texture* m_texture;
	bool m_flipV;
	bool m_flipH;
	utils::VertexBufferWraper m_buffer { sf::Triangles, sf::VertexBuffer::Static, 6U };
};

_R2D_COMP_NS_END_
