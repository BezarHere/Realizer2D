#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

class Object2D;
class ObjectComponent
{
	friend class Object2D;
public:
	ObjectComponent();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const {}

	inline const Object2D* getOwner() const { return m_owner; }

	virtual void setActive(bool active);
	inline bool isActive() const { return m_active; }

protected:
	virtual void update(real_t delta) {}

	// called if the owner is deactivated globaly
	virtual void ownerVisiblityChangeCallback();

	// called BEFORE the owner is detached
	virtual void ownerDetachedCallback();

	// called AFTER the owner is attached
	virtual void ownerAtachedCallback();

private:
	bool m_active{ true };
	Object2D* m_owner{ nullptr };
};

_R2D_NAMESPACE_END_