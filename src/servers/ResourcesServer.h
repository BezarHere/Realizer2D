#pragma once
#include "global.h"
#include "resource/Resource.h"

_R2D_NAMESPACE_START_

class ResourcesServer final
{

	ResourcesServer() = delete;
private:
	static inline std::unordered_map<size_t, Reousrce> s_cache;
};

_R2D_NAMESPACE_END_
