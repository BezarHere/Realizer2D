#include "global.h"
#include "servers/VisualServer.h"
#include "Camera.h"


_R2D_COMP_NS_START_

Camera::~Camera()
{
	// might be buggey? if the mem is not set to nullptr after this current cam is deleted then undefined behivore will occure.
	if (isCurrent())
	{
		_R2D_ VisualServer::MakeCameraCurrent(nullptr);
	}
}

void Camera::setZoom(Vector2 zoom)
{
	m_zoom = zoom;
}

void Camera::setZoom(real_t x, real_t y)
{
	m_zoom = { x, y };
}

void Camera::setZoom(real_t zoom)
{
	m_zoom = { zoom, zoom };
}

Vector2 Camera::getZoom() const
{
	return m_zoom;
}

void Camera::setPosition(Vector2 position)
{
	m_position = position;
}

void Camera::setPosition(real_t x, real_t y)
{
	m_position = { x, y };
}

Vector2 Camera::getPosition() const
{
	return m_position;
}

void Camera::setCentered(bool centered)
{
	m_centered = centered;
}

bool Camera::getCentered() const
{
	return m_centered;
}

void Camera::makeCurrent()
{
	m_current = true;
	if (!getOwner())
	{
		//_r2d_warning("can't make an orphan camera as the active current camera.");
		return;
	}
	_R2D_ VisualServer::MakeCameraCurrent(this);
}

bool Camera::getCurrent() const
{
	return m_current;
}

bool Camera::isCurrent() const
{
	return _R2D_ VisualServer::GetCamera() == this;
}

uint32_t Camera::getSingleton() const
{
	return 2U;
}

void Camera::tryToPopFromCurrentCamera()
{
	if (isCurrent())
	{
		_R2D_ VisualServer::MakeCameraCurrent(nullptr);
	}
}

void Camera::tryToGainCurrentCamera()
{
	if (getCurrent() && !_R2D_ VisualServer::GetCamera())
	{
		makeCurrent();
	}
}

void Camera::nolongerCurrent()
{
	m_current = false;
}

void Camera::onOwnerDetached()
{
	tryToPopFromCurrentCamera();
}

void Camera::onOwnerAtached()
{
	tryToGainCurrentCamera();
}

void Camera::entredScene()
{
	tryToGainCurrentCamera();
}

void Camera::exitedScene()
{
	tryToPopFromCurrentCamera();
}

_R2D_COMP_NS_END_

_R2D_register_comp(Camera);

