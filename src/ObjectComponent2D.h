#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

class Object2D;

class ObjectComponent2D
{
public:
	friend class Object2D;
	virtual void update() {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {}

	inline const Object2D* getObject() const { return m_obj; }
	inline void setActive(bool active) { m_active = active; }
	inline bool isActive() const { return m_active; }

private:
	bool m_active;
	Object2D* m_obj;
};

_R2D_NAMESPACE_END_