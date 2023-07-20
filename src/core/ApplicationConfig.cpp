#include "global.h"
#include "ApplicationConfig.h"

_R2D_NAMESPACE_START_

ApplicationConfig::ApplicationConfig()
{
}

void ApplicationConfig::setTitle(std::string title)
{
	m_title = title;
}

const std::string& ApplicationConfig::getTitle() const
{
	return m_title;
}

void ApplicationConfig::setStartWidowSize(Size2 size)
{
	m_startWindowSize = size;
}

Size2 ApplicationConfig::getStartWidowSize() const
{
	return m_startWindowSize;
}

void ApplicationConfig::setStretchMode(ViewStretchMode mode)
{
	m_stretchMode = mode;
}

ViewStretchMode ApplicationConfig::getStretchMode() const
{
	return m_stretchMode;
}

void ApplicationConfig::setClearColor(Color clr)
{
	m_clearColor = clr;
}

Color ApplicationConfig::getClearColor() const
{
	return m_clearColor;
}

void ApplicationConfig::makeMaster()
{
	ReplaceMasterConfig(*this);
}

void ApplicationConfig::ReplaceMasterConfig(ApplicationConfig& config)
{
	if (&s_masterConfig == &config)
		return;
	s_masterConfig.removedFromMaster();
	s_masterConfig = config;
}

ApplicationConfig& ApplicationConfig::MasterConfig()
{
	return s_masterConfig;
}

ApplicationConfig& ApplicationConfig::CreateInit()
{
	s_masterConfigPuppet = new ApplicationConfig();
	return *s_masterConfigPuppet;
}

void ApplicationConfig::removedFromMaster()
{
}

_R2D_NAMESPACE_END_