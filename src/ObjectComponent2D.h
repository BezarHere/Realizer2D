#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

class Object2D;
class ObjectComponent2D
{
	friend class Object2D;
public:
	ObjectComponent2D();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {}

	inline const Object2D* getObject() const { return m_obj; }
	inline void setActive(bool active) { m_active = active; }
	inline bool isActive() const { return m_active; }

	inline virtual uint32_t getId() const = 0;

protected:
	virtual void update(real_t delta) {}

	ObjectComponent2D(const uint64_t uid);
	const uint64_t unique_id;
private:
	bool m_active;
	Object2D* m_obj;
};

_R2D_NAMESPACE_END_