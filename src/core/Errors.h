#pragma once
#include "../global.h"

_R2D_NAMESPACE_START_

// why the need in a man for a surpluse of errors, 65536 errors shall suffice
enum Error : uint16_t
{
	OK,
	FAILED,
	ERR_NAME_ALREADY_EXISTS,
	ERR_ALREADY_EXISTS
};

_R2D_NAMESPACE_END_