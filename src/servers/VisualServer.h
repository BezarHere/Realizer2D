#pragma once
#include <stack>
#include <set>
#include "global.h"
#include "core/ApplicationConfig.h"
#include "scene/SceneTree.h"
#include "components/Graphical.h"
#include "components/Gui.h"
#include "components/Camera.h"

_R2D_NAMESPACE_START_
#define global static inline

class VisualServer final
{
	friend class Engine;
public:
	static inline sf::RenderWindow* GetWindow() { return VisualServer::s_window; }

	static void SetDrawingAction(DrawerFunction_t action);
	static inline DrawerFunction_t GetDrawingAction() { return s_drawAction; }
	
	static void start();
	
	// automaticlty updated
	// ** only call if you know what you're doing **

	static void updateView();

	// same as calling make current on the camera
	static void MakeCameraCurrent(components::Camera* camera);
	static components::Camera* GetCamera();

	static Vector2 GetScreenSize();
	// the topleft position in global coords
	static Vector2 GetScreenOrigin();

	struct _ZHeightElement
	{


		inline bool operator==(const _ZHeightElement& other) const
		{
			return object == other.object;
		}

		inline bool operator<(const _ZHeightElement& other) const
		{
			return object->getZIndex() < other.object->getZIndex();
		}

		inline bool operator>(const _ZHeightElement& other) const
		{
			return object->getZIndex() > other.object->getZIndex();
		}

		Object2D* object;
	};

private:
	class VSRenderWindow : public sf::RenderWindow
	{
	public:
		VSRenderWindow(){}
		VSRenderWindow(
			sf::VideoMode mode,
			const std::string& title,
			uint32_t style = sf::Style::Default,
			const sf::ContextSettings& settings = sf::ContextSettings()
			) : RenderWindow(mode, title, style, settings) {}
	protected:
		inline void onResize() override { VisualServer::ScreenResized(); }
	};

	VisualServer() = delete;

	// safe way, making the default cam visible in the normal get camera function might be buggey.
	static components::Camera* GetCameraInt();

	static void ScreenResized();
	static void PreDraw();
	static void DoDraw();

private:
	global sf::RenderTexture s_drawTexture;
	global VSRenderWindow* s_window{ nullptr };
	global DrawerFunction_t s_drawAction;
	global const ApplicationConfig m_appliedConfig;
	global const ViewStretchMode m_viewStretchMode = ViewStretchMode::Expand;
	global Vector2 s_screenSize{};
	global Vector2 s_screenTopleft{};
	global sf::RenderStates s_worldRenderStates;
	global sf::RenderStates s_screenRenderStates;
	global Size2 s_startWindowSize;
	global components::Camera s_defaultCamera{};
	global components::Camera* s_currentCamera = nullptr;

	// current way of handling z-index

};


#undef global
_R2D_NAMESPACE_END_

template <> struct std::hash<r2d::VisualServer::_ZHeightElement> {
	inline size_t operator()(const r2d::VisualServer::_ZHeightElement& obj) const {
		return (size_t)obj.object;
	}
};
