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

	// if true, any valid cached resource getting loaded will be loaded from cache
	static bool CanReadFromCache();
	// if true, any valid loading of a new resource will be parsed to cache
	static bool CanWriteToCache();

	// wont clear the cache
	static void SetReadToCache(bool value);

	// wont clear the cache
	static void SetWriteToCache(bool value);

	// invalidates the resource's cache, the cache is still there
	static ErrorCode InvalidateCache(hash_t hash);
	static ErrorCode InvalidateCache(const std::string &path);

	// streamed resources are always loaded from disk
	static ErrorCode ChangeResourceStreamFlags(const std::string &path, bool streamed);
	static bool IsResourceStreamed(const std::string &path);

	static const Font &LoadFont(const std::string &filepath);
	static ErrorCode LoadFont(const std::string &filepath, Font &font);

	static const Font& GetDefaultFont();

private:
	// Should stay until program's termination
	struct ResourceSign
	{
		void *data; //! bad
		std::string path;
		bool valid{ true };
		bool streamed{ false }; // makes the cache unusable
	};
	ResourcesServer() = delete;

	static ErrorCode Init();

	static bool HasCached(hash_t hash);
	static ErrorCode AddCache(hash_t hash, const ResourceSign& res);

	static ErrorCode TryPutCache(void *data, const std::string &path);
	static ResourceSign *TryGetCache(const hash_t hash);
	static ResourceSign *TryGetCache(const std::string &path);

private:
	global std::unordered_map<hash_t, ResourceSign> s_cache;
	global bool s_cacheWrite{ true };
	global bool s_cacheRead{ true };
	global Font s_defaultFont;
	global bool s_initFuncCalled{ false };
};

#undef global
_R2D_NAMESPACE_END_
