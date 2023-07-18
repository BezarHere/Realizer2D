#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

struct Resource
{

};

class ResourcesServer final
{

	ResourcesServer() = delete;
private:
	static inline std::unordered_map<size_t, Resource> s_cache;
};

_R2D_NAMESPACE_END_
