#include "global.h"
#include "ResourcesServer.h"

_R2D_NAMESPACE_START_

__forceinline void f_santize_path(char *path)
{
	for (size_t i{}; path[ i ]; i++)
	{
		if (path[ i ] == '/')
			path[ i ] = '\\';
#ifdef WIN32
		// make path lowercase in windows
		if (path[ i ] >= 'A' && path[ i ] <= 'Z')
			path += 'a' - 'A';
#endif // WIN32
	}
}

__forceinline size_t &&f_hash_path(char *path)
{
	f_santize_path(path);
	return hash(path);
}

__forceinline size_t &&f_hash_path(std::string path)
{
	f_santize_path(path.data());
	return hash(std::forward<std::string>(path));
}

bool ResourcesServer::CanReadFromCache()
{
	return s_cacheRead;
}

bool ResourcesServer::CanWriteToCache()
{
	return s_cacheWrite;
}

void ResourcesServer::SetReadToCache(bool value)
{
	s_cacheRead = value;
}

void ResourcesServer::SetWriteToCache(bool value)
{
	s_cacheWrite = value;
}

ErrorCode ResourcesServer::ChangeResourceStreamFlags(const std::string &path, bool streamed)
{
	hash_t hash{ f_hash_path(path) };
	if (!HasCached(hash))
		return ErrorCode::KeyDoesNotExist;
	s_cache.at(hash).streamed = streamed;
	return ErrorCode::Ok;
}

bool ResourcesServer::IsResourceStreamed(const std::string &path)
{
	hash_t hash{ f_hash_path(path) };
	return HasCached(hash) && s_cache.at(hash).streamed;
}

const Font& ResourcesServer::LoadFont(const std::string& filepath)
{
	Font font = Font{};
	(void)LoadFont(filepath, font);
	return font;
}

ErrorCode ResourcesServer::LoadFont(const std::string& filepath, Font& font)
{
	if (!font.loadFromFile(filepath))
	{
		_r2d_error("couldn't load font from path: " + filepath);
		return ErrorCode::CouldNotLoad;
	}
	return ErrorCode::Ok;
}

const Font& ResourcesServer::GetDefaultFont()
{
	return s_defaultFont;
}

ErrorCode ResourcesServer::Init()
{
	#define errcheck if (err != ErrorCode::Ok) return err
	ErrorCode err{};
	err = LoadFont("res/font.ttf", s_defaultFont);
	errcheck;

	return ErrorCode::Ok;
}

bool ResourcesServer::HasCached(hash_t hash)
{
	return s_cache.contains(hash);
}

ErrorCode ResourcesServer::AddCache(hash_t hash, const ResourceSign &res)
{
	if (HasCached(hash))
		return ErrorCode::ValueAlreadyExists;
	s_cache.insert_or_assign(hash, res);
	return ErrorCode::Ok;
}

ErrorCode ResourcesServer::TryPutCache(void *data, const std::string &path)
{
	s_cache.insert_or_assign(
		f_hash_path(path), ResourceSign{ data, path, true, false }
	);
	return ErrorCode::Ok;
}

ResourcesServer::ResourceSign *ResourcesServer::TryGetCache(const hash_t hash)
{
	if (s_cacheRead || !HasCached(hash))
		return nullptr;
	return &s_cache.at(hash);
}

ResourcesServer::ResourceSign *ResourcesServer::TryGetCache(const std::string &path)
{
	return TryGetCache(f_hash_path(path));
}

ErrorCode ResourcesServer::InvalidateCache(hash_t hash)
{
	if (!HasCached(hash))
		return ErrorCode::ValueDoesNotExist;
	s_cache.at(hash).valid = false;
	return ErrorCode::Ok;
}

ErrorCode ResourcesServer::InvalidateCache(const std::string &path)
{
	return InvalidateCache(f_hash_path(path));
}


_R2D_NAMESPACE_END_