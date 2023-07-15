#include "global.h"
#include "VisualServer.h"

_R2D_NAMESPACE_START_

void VisualServer::SetDrawingAction(DrawerFunction_t action)
{
	s_drawAction = action;
}

void VisualServer::update()
{
	const auto& objs = SceneTree::GetRootObjects();
	std::vector<_ZHeightElement> obj_z_order;
	sf::RenderWindow& window = *VisualServer::GetWindow();

	for (Object2D* p : objs)
	{
		obj_z_order.push_back(_ZHeightElement(p));
	}

	std::sort(obj_z_order.begin(), obj_z_order.end());

	for (const _ZHeightElement& p : obj_z_order)
	{
		p.object->draw(window, m_worldRenderStates);
	}


	if (s_drawAction)
		s_drawAction(window, m_worldRenderStates);
}

void VisualServer::updateView()
{
	sf::View pview = s_window->getView();
	sf::Vector2f window_size = (sf::Vector2f)s_window->getSize();
	switch (m_viewStretchMode)
	{
		case ViewStretchMode::Expand:
			pview.setSize(window_size / m_viewZoom);
			break;
		case ViewStretchMode::KeepHorizontal:
			pview.setSize(sf::Vector2f((float)s_startWindowSize.x, window_size.y) / m_viewZoom);
			break;
		case ViewStretchMode::KeepVertical:
			pview.setSize(sf::Vector2f(window_size.x, (float)s_startWindowSize.y) / m_viewZoom);
			break;
		case ViewStretchMode::Keep:
			pview.setSize((sf::Vector2f)s_startWindowSize / m_viewZoom);
			break;
		default:
			break;
	}
	
	if (m_viewCentered)
	{
		pview.setCenter(m_viewPosition);
	}
	else
	{
		pview.setCenter(m_viewPosition + (pview.getSize() / 2.0f));
	}

	s_window->setView(pview);
}

void VisualServer::setViewPosition(sf::Vector2f position)
{
	m_viewPosition = position;
	updateView();
}

void VisualServer::moveView(sf::Vector2f offset)
{
	m_viewPosition += offset;
	updateView();
}

void VisualServer::setViewZoom(sf::Vector2f amount)
{
	m_viewZoom = amount;
	updateView();
}

void VisualServer::setViewZoom(float amount)
{
	m_viewZoom = sf::Vector2f(amount, amount);
	updateView();
}

void VisualServer::setViewCentered(bool centered)
{
	if (m_viewCentered == centered)
		return;
	m_viewCentered = centered;
	updateView();
}

void VisualServer::ScreenResized()
{
	updateView();
}

void VisualServer::PreDraw()
{
	s_window->clear(ApplicationConfig::MasterConfig().getClearColor());
}



void VisualServer::start()
{
	ScreenResized();
	sf::VideoMode vmode{ ApplicationConfig::MasterConfig().getStartWidowSize().x, ApplicationConfig::MasterConfig().getStartWidowSize().y, 8};
	sf::ContextSettings vcontext{ 8U, 8U, 0U };
	s_window = new VSRenderWindow(vmode, ApplicationConfig::MasterConfig().getTitle(), sf::Style::Default, vcontext);
	s_startWindowSize = ApplicationConfig::MasterConfig().getStartWidowSize();
	
}

_R2D_NAMESPACE_END_