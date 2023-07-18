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
	inline bool isOrphan() const { return m_owner == nullptr; }
	//inline bool isFrozen() const { return !isOrphan() && m_owner->isInsideScene(); }

	virtual void setActive(bool active);
	inline bool isActive() const { return m_active; }
	
	// weather this type of component can be added multiple times
	// the value also is like... the component singleton id?
	virtual uint32_t getSingleton() const;

protected:
	virtual void update(real_t delta) {}

	// called if the owner is deactivated globaly
	virtual void onOwnerVisiblityChanged();

	// called BEFORE the owner is detached
	virtual void onOwnerDetached();

	// called AFTER the owner is attached
	virtual void onOwnerAtached();

	// will not get called when this is added to an object the is INSIDE the scene
	virtual void entredScene();

	// will not get called when this is added to an object the is OUTSIDE the scene
	// called BEFORE exiting the scene
	virtual void exitedScene();


private:
	bool m_active{ true };
	Object2D* m_owner{ nullptr };
};

_R2D_NAMESPACE_END_