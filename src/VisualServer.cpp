#include "global.h"
#include "VisualServer.h"

_R2D_NAMESPACE_START_

VisualServer::VisualServer()
	: m_viewStretchMode(s_config.stretchMode),
		m_appliedConfig(VisualServer::s_config)
{
	screenResized();
}

void VisualServer::update()
{
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
			pview.setSize(sf::Vector2f(m_appliedConfig.windowSize.x, window_size.y) / m_viewZoom);
			break;
		case ViewStretchMode::KeepVertical:
			pview.setSize(sf::Vector2f(window_size.x, m_appliedConfig.windowSize.y) / m_viewZoom);
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