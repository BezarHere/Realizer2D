#include "global.h"
#include "VisualServer.h"

_R2D_NAMESPACE_START_

void VisualServer::SetDrawingAction(DrawerFunction_t action)
{
	s_drawAction = action;
}

void VisualServer::DoDraw()
{
	const auto& objs = SceneTree::GetRootObjects();
	std::vector<_ZHeightElement> obj_z_order;
	sf::RenderWindow& window = *VisualServer::GetWindow();
	updateView();

	s_screenSize = window.getView().getSize();
	s_screenTopleft = window.getView().getCenter() - (s_screenSize / 2.0f);

	for (Object2D* p : objs)
	{
		obj_z_order.push_back(_ZHeightElement(p));
	}

	// TODO: add a more efficent sort
	std::sort(obj_z_order.begin(), obj_z_order.end());

	for (const _ZHeightElement& p : obj_z_order)
	{
		p.object->draw(window, p.object->getUsesRelativeCoords() ? s_worldRenderStates : s_screenRenderStates);
	}


	if (s_drawAction)
		s_drawAction(window, s_worldRenderStates);

	// Post-processing:

	/*sf::Texture screen_texture{};
	screen_texture.create(window.getSize().x, window.getSize().y);
	screen_texture.update(window);*/
	window.display();
}

void VisualServer::updateView()
{
	sf::View pview = s_window->getView();
	Vector2 window_size = (Vector2)s_window->getSize();
	components::Camera* cam = GetCameraInt();
	Vector2 cam_global_pos = cam == &s_defaultCamera ? cam->getPosition() : cam->getOwner()->getGlobalPosition() + cam->getPosition();

	switch (m_viewStretchMode)
	{
		case ViewStretchMode::Expand:
			pview.setSize(window_size / cam->getZoom());
			break;
		case ViewStretchMode::KeepHorizontal:
			pview.setSize(sf::Vector2f((float)s_startWindowSize.x, window_size.y) / cam->getZoom());
			break;
		case ViewStretchMode::KeepVertical:
			pview.setSize(sf::Vector2f(window_size.x, (float)s_startWindowSize.y) / cam->getZoom());
			break;
		case ViewStretchMode::Keep:
			pview.setSize((sf::Vector2f)s_startWindowSize / cam->getZoom());
			break;
		default:
			break;
	}
	
	if (GetCameraInt()->getCentered())
	{
		pview.setCenter(cam_global_pos);
	}
	else
	{
		pview.setCenter(cam_global_pos + (pview.getSize() / 2.0f));
	}

	s_screenRenderStates.transform = Transform2D().translate(pview.getCenter() - (pview.getSize() / 2.0f));
	s_window->setView(pview);
}

void VisualServer::MakeCameraCurrent(components::Camera* camera)
{
	if (camera == s_currentCamera)
	{
		return;
	}

	if (s_currentCamera)
	{
		s_currentCamera->nolongerCurrent();
	}

	if (camera != nullptr)
	{
		// orphan camera, no good
		if (camera->isOrphan())
		{
			_r2d_error("can't make an orphan camera to be the current camera (bug?).");
			return;
		}
		// idle camera, no good
		else if (!camera->getOwner()->isInsideScene())
		{
			_r2d_error("can't make a camera to be the current camera when it's not inside the scene (bug?).");
			return;
		}
	}

	s_currentCamera = camera;
}

components::Camera* VisualServer::GetCamera()
{
	return s_currentCamera;
}

Vector2 VisualServer::GetScreenSize()
{
	return s_screenSize;
}

Vector2 VisualServer::GetScreenOrigin()
{
	return s_screenTopleft;
}

components::Camera* VisualServer::GetCameraInt()
{
	if (s_currentCamera == nullptr)
		return &s_defaultCamera;
	return GetCamera();
}

void VisualServer::ScreenResized()
{
	updateView();
}

void VisualServer::PreDraw()
{
	s_window->clear(ApplicationConfig::MasterConfig().getClearColor());
	//s_drawTexture.create(s_window->getSize().x, s_window->getSize().y);
}

void VisualServer::start()
{
	static constexpr std::pair<uint32_t, uint32_t> MasStartScreenSize{ 1 << 13 , 1 << 12 };
	if (ApplicationConfig::MasterConfig().getStartWidowSize().x > MasStartScreenSize.first || ApplicationConfig::MasterConfig().getStartWidowSize().y > MasStartScreenSize.second)
	{
		_r2d_warning(
			"Can't set the window start size in the application settings to be larger then (" + std::to_string(MasStartScreenSize.first) + ", " + std::to_string(MasStartScreenSize.second) + ")");
		ApplicationConfig::MasterConfig().setStartWidowSize({
			min(MasStartScreenSize.first, ApplicationConfig::MasterConfig().getStartWidowSize().x),
			min(MasStartScreenSize.second, ApplicationConfig::MasterConfig().getStartWidowSize().y),
		});
	}

	sf::VideoMode vmode{ ApplicationConfig::MasterConfig().getStartWidowSize().x, ApplicationConfig::MasterConfig().getStartWidowSize().y, 8};
	sf::ContextSettings vcontext{ 8U, 8U, 0U };
	
	s_window = new VSRenderWindow(vmode, ApplicationConfig::MasterConfig().getTitle(), sf::Style::Default, vcontext);;
	s_startWindowSize = ApplicationConfig::MasterConfig().getStartWidowSize();
	ScreenResized();
	
}

_R2D_NAMESPACE_END_