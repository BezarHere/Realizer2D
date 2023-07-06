#pragma once
#include <stack>
#include <set>
#include "global.h"
#include "ApplicationConfig.h"
#include "SceneTree.h"
#include "components/Drawers.h"

_R2D_NAMESPACE_START_

class VisualServer final
{
public:
	static inline VisualServer* Singleton() { return VisualServer::s_instance; }
	static inline sf::RenderWindow* GetWindow() { return (sf::RenderWindow*)VisualServer::s_window; }

	static void SetDrawingAction(DrawerFunction_t action);
	static inline DrawerFunction_t GetDrawingAction() { return s_drawAction; }
	
	static void start();
	
	// automaticlty updated
	// ** only call if you know what you're doing **
	void update();

	void updateView();
	
	void setViewPosition(sf::Vector2f position);
	void moveView(sf::Vector2f offset);
	void setViewZoom(sf::Vector2f amount);
	void setViewZoom(float amount);
	void setViewCentered(bool centered);

	inline sf::Vector2f getViewPosition() const { return m_viewPosition; }
	inline sf::Vector2f getViewZoom() const { return m_viewZoom; }
	inline bool isViewCentered() const { return m_viewCentered; }

	static inline void setApplicationConfig(ApplicationConfig config)
	{
		s_config = config;
	}
	static inline ApplicationConfig& getApplicationConfig()
	{
		return s_config;
	}

public:
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
		inline void onResize() override { VisualServer::Singleton()->screenResized(); }
	};

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

	

	VisualServer();

	void screenResized();

private:
	static inline VisualServer* s_instance;
	static inline VSRenderWindow* s_window;
	static inline ApplicationConfig s_config;
	static inline DrawerFunction_t s_drawAction;
	const ApplicationConfig m_appliedConfig;
	const ViewStretchMode m_viewStretchMode = ViewStretchMode::Expand;
	sf::Vector2f m_viewZoom = sf::Vector2f(1.0f, 1.0f);
	sf::Vector2f m_viewPosition = sf::Vector2f(0.0f, 0.0f);
	// if false; the view will be anchored at the topleft
	bool m_viewCentered = true;
	sf::RenderStates m_worldRenderStates;
	sf::RenderStates m_screenRenderStates;

	// current way of handling z-index

};



_R2D_NAMESPACE_END_

template <> struct std::hash<r2d::VisualServer::_ZHeightElement> {
	inline size_t operator()(const r2d::VisualServer::_ZHeightElement& obj) const {
		return (size_t)obj.object;
	}
};
