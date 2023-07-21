#include "global.h"
#include "ResourcesServer.h"

_R2D_NAMESPACE_START_

const Font& ResourcesServer::LoadFont(const std::string& filepath)
{
	Font font{};
	(void)LoadFont(filepath, font);
	return font;
}

Error ResourcesServer::LoadFont(const std::string& filepath, Font& font)
{
	if (!font.loadFromFile(filepath))
	{
		_r2d_error("couldn't load font from path: " + filepath);
		return Error::CouldntLoad;
	}
	return Error::Ok;
}

const Font& ResourcesServer::GetDefaultFont()
{
	return s_defaultFont;
}

Error ResourcesServer::Init()
{
	#define errcheck if (err != Error::Ok) return err
	Error err{};
	err = LoadFont("res/font.ttf", s_defaultFont);
	errcheck;

	return Error::Ok;
}

_R2D_NAMESPACE_END_