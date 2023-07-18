#pragma once
#include "global.h"
#include "base.h"

_R2D_NAMESPACE_START_
class VisualServer;
_R2D_NAMESPACE_END_

_R2D_COMP_NS_START_

class Camera : public ObjectComponent
{
	friend class VisualServer;
public:
	// make sure the camera value in visual server will not point to invalid memory
	~Camera();

	void setZoom(Vector2 zoom);
	void setZoom(real_t x, real_t y);
	void setZoom(real_t zoom);
	Vector2 getZoom() const;

	void setPosition(Vector2 position);
	void setPosition(real_t x, real_t y);
	Vector2 getPosition() const;

	void setCentered(bool centered);
	bool getCentered() const;

	// only way to *unmake* it current is to make another camera current (idk im half sleep)
	void makeCurrent();
	// if true then the camera is considring itself current if it's not becuse it's not inside the scene then 
	// it will try to control the view of the scene if no camera is current already as soon it's added to the scene
	bool getCurrent() const;
	// if true then the camera is active and controling view inside the scene
	bool isCurrent() const;

	uint32_t getSingleton() const override;

private:
	static inline void test(){};

	void tryToPopFromCurrentCamera();
	void tryToGainCurrentCamera();

	void nolongerCurrent();

	void onOwnerDetached() override;
	void onOwnerAtached() override;
	void entredScene() override;
	void exitedScene() override;

private:
	bool m_centered{ true };
	bool m_current{ false };
	Vector2 m_zoom{ 1.0f, 1.0f };
	Vector2 m_position{ 0.0f, 0.0f };
};

_R2D_COMP_NS_END_