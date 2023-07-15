#include "global.h"
#include "Application.h"

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
	PushToConfigStack(*this);
}

void ApplicationConfig::PushToConfigStack(ApplicationConfig& config)
{
	ApplicationConfig* config_ptr = &config;
	for (const ApplicationConfig& i : s_configStack._Get_container())
	{
		// possibly adding a config that is already inside
		if (&i == config_ptr)
		{
			_pr_warning("Failed to add config to the stack: apllication config already inside the config stack!");
			return;
		}
	}
	s_configStack.push(config);
}

ApplicationConfig& ApplicationConfig::PopMasterConfig()
{
	ApplicationConfig& master = MasterConfig();
	RemoveMasterConfig();
	return master;
}

ApplicationConfig& ApplicationConfig::MasterConfig()
{
	return s_configStack.top();
}

void ApplicationConfig::RemoveMasterConfig()
{
	s_configStack.pop();
	ValidateMasterStack();
}

const std::stack<ApplicationConfig> ApplicationConfig::GetConfigStack()
{
	return s_configStack;
}

std::stack<ApplicationConfig> ApplicationConfig::CreateInitStack()
{
	std::stack<ApplicationConfig> stack;
	ApplicationConfig starter;
	stack.push(starter);
	return stack;
}

void ApplicationConfig::ValidateMasterStack(bool show_warning)
{
	if (s_configStack.empty())
	{
		if (show_warning)
		{
			_pr_warning("Master config stack is emptied: the used master application config for all the engine processes is being removed, a default application config is now the master config.");
		}
		ApplicationConfig default_cfg;
		s_configStack.push(default_cfg);
	}
}

void ApplicationConfig::removedFromMaster()
{
}

_R2D_NAMESPACE_END_