#pragma once
#include "global.h"
#include "resource/Resource.h"
#include "core/Errors.h"
#include "R2DInit.h"

_R2D_NAMESPACE_START_
#define global static inline

class ResourcesServer final
{
	friend class R2DInit;
public:

	static const Font& LoadFont(const std::string& filepath);
	static Error LoadFont(const std::string& filepath, Font& font);

	static const Font& GetDefaultFont();

private:
	ResourcesServer() = delete;

	static Error Init();

private:
	global std::unordered_map<size_t, void*> s_cache;
	global Font s_defaultFont;
	global bool s_initFuncCalled{ false };
};

#undef global
_R2D_NAMESPACE_END_
