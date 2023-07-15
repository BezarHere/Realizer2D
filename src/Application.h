#pragma once
#include "global.h"
#include "core/StretchMode.h"
#include <stack>

_R2D_NAMESPACE_START_

class ApplicationConfig
{
	friend class Engine;
public:
	ApplicationConfig();

	void setTitle(std::string title);
	const std::string& getTitle() const;

	void setStartWidowSize(Size2 size);
	Size2 getStartWidowSize() const;

	void setStretchMode(ViewStretchMode mode);
	ViewStretchMode getStretchMode() const;

	void setClearColor(Color clr);
	Color getClearColor() const;

	// apply this config to be the master one, wich the engine will use
	// this will override the master app config but this WILL NOT REFREANCE THE MASTER CONFIG
	// so any changes to this config after making it master will not change the actual master config
	// if you wanted to change the master config then change it from the static MasterConfig() function
	void makeMaster();

	// adds the given config to the top of the config stack wich will make it the master config used by all the engine processes
	static void PushToConfigStack(ApplicationConfig& config);

	// removes and returns the top master config from the stack ( the top master config is the one used by the engine processes )
	// if the master config being removed is the last one on the stack, a defualt config will be pushed to replace it
	[[nodiscard]] static ApplicationConfig& PopMasterConfig();

	// returns the top master config from the stack ( the top master config is the one used by the engine processes )
	[[nodiscard]] static ApplicationConfig& MasterConfig();

	// removes the top master config from the stack ( the top master config is the one used by the engine processes )
	// if the master config being removed is the last one on the stack, a defualt config will be pushed to replace it
	static void RemoveMasterConfig();

	// be careful
	static const std::stack<ApplicationConfig> GetConfigStack();

private:
	static std::stack<ApplicationConfig> CreateInitStack();
	static void ValidateMasterStack(bool show_warning = true);
	void removedFromMaster();

private:
	static inline std::stack<ApplicationConfig> s_configStack = CreateInitStack();
	std::string m_title { "Title" };
	Color m_clearColor {0, 0, 0};
	// the window size at program init
	Size2 m_startWindowSize {512u, 400u};
	ViewStretchMode m_stretchMode { ViewStretchMode::Expand };
};

_R2D_NAMESPACE_END_