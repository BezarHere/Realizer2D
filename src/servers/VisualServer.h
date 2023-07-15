#pragma once
#include <stack>
#include <set>
#include "global.h"
#include "Application.h"
#include "scene/SceneTree.h"
#include "components/Drawers.h"

_R2D_NAMESPACE_START_
#define global static inline

class VisualServer final
{
	friend class Engine;
public:
	static inline VisualServer* Singleton() { return VisualServer::s_instance; }
	static inline sf::RenderWindow* GetWindow() { return (sf::RenderWindow*)VisualServer::s_window; }

	static void SetDrawingAction(DrawerFunction_t action);
	static inline DrawerFunction_t GetDrawingAction() { return s_drawAction; }
	
	static void start();
	
	// automaticlty updated
	// ** only call if you know what you're doing **
	static void update();

	static void updateView();
	
	static void setViewPosition(sf::Vector2f position);
	static void moveView(sf::Vector2f offset);
	static void setViewZoom(sf::Vector2f amount);
	static void setViewZoom(float amount);
	static void setViewCentered(bool centered);

	global sf::Vector2f getViewPosition() { return m_viewPosition; }
	global sf::Vector2f getViewZoom() { return m_viewZoom; }
	global bool isViewCentered() { return m_viewCentered; }

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
		VSRenderWindow(
			sf::VideoMode mode,
			const std::string& title,
			uint32_t style = sf::Style::Default,
			const sf::ContextSettings& settings = sf::ContextSettings()
			) : RenderWindow(mode, title, style, settings) {}
	protected:
		inline void onResize() override { VisualServer::ScreenResized(); }
	};

	VisualServer() = 0;

	static void ScreenResized();
	static void PreDraw();

private:
	global VSRenderWindow* s_window;
	global DrawerFunction_t s_drawAction;
	global const ApplicationConfig m_appliedConfig;
	global const ViewStretchMode m_viewStretchMode = ViewStretchMode::Expand;
	global Vector2 m_viewZoom = sf::Vector2f(1.0f, 1.0f);
	global Vector2 m_viewPosition = sf::Vector2f(0.0f, 0.0f);
	global Vector2 s_screenSize{};
	global Vector2 s_screenTopleft{};
	// if false; the view will be anchored at the topleft
	global bool m_viewCentered = true;
	global sf::RenderStates m_worldRenderStates;
	global sf::RenderStates m_screenRenderStates;
	global Size2 s_startWindowSize;

	// current way of handling z-index

};


#undef global
_R2D_NAMESPACE_END_

template <> struct std::hash<r2d::VisualServer::_ZHeightElement> {
	inline size_t operator()(const r2d::VisualServer::_ZHeightElement& obj) const {
		return (size_t)obj.object;
	}
};
