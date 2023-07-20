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

	// Makes the config master, replacing the old master config
	// the master config is a refreance so any changes to this will it's the master config
	// will change the master config
	void makeMaster();

	// replaces the master config ( wich is used by the engine processes )
	static void ReplaceMasterConfig(ApplicationConfig& config);

	// returns the master config ( wich is used by the engine processes )
	[[nodiscard]] static ApplicationConfig& MasterConfig();

private:
	static ApplicationConfig& CreateInit();
	void removedFromMaster();

private:
	static inline ApplicationConfig* s_masterConfigPuppet;
	static inline ApplicationConfig& s_masterConfig = CreateInit();
	std::string m_title { "Title" };
	Color m_clearColor {0, 0, 0};
	// the window size at program init
	Size2 m_startWindowSize {512u, 400u};
	ViewStretchMode m_stretchMode { ViewStretchMode::Expand };
	bool m_physicsMultiThreaded{ false };

};

_R2D_NAMESPACE_END_