#pragma once
#include "../global.h"

_R2D_NAMESPACE_START_

enum class Error : uint32_t
{
	Ok,
	Failed,
	NameAlreadyExists,
	AlreadyExists,
	Max // meaningless.
};

extern const std::string& GetErrorName(Error error);


_R2D_NAMESPACE_END_