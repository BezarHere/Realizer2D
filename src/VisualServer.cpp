#include "global.h"
#include "VisualServer.h"

_R2D_NAMESPACE_START_

VisualServer::VisualServer()
	: m_viewStretchMode(s_config.stretchMode),
		m_appliedConfig(VisualServer::s_config)
{
	screenResized();
}

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
			pview.setSize(sf::Vector2f((float)m_appliedConfig.windowSize.x, window_size.y) / m_viewZoom);
			break;
		case ViewStretchMode::KeepVertical:
			pview.setSize(sf::Vector2f(window_size.x, (float)m_appliedConfig.windowSize.y) / m_viewZoom);
			break;
		case ViewStretchMode::Keep:
			pview.setSize((sf::Vector2f)m_appliedConfig.windowSize / m_viewZoom);
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

void VisualServer::screenResized()
{
	updateView();
}



void VisualServer::start()
{
	assert_msg(!s_instance, "can't start the visual server: already started")
	sf::VideoMode vmode{ s_config.windowSize.x, s_config.windowSize.y, 8 };
	sf::ContextSettings vcontext{ 8U, 8U, 0U };
	s_window = new VSRenderWindow(vmode, s_config.title, sf::Style::Default, vcontext);
	s_instance = new VisualServer();
	
}

_R2D_NAMESPACE_END_