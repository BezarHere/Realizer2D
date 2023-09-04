#pragma once
#include "../global.h"

_R2D_NAMESPACE_START_

// error codes should convay as much information as possible in their names, names can be up to 38 charcters
// examble of max length error code: NoMoreCackesFoundForTheOpenOvenTonight
enum class ErrorCode : uint32_t
{
	Ok,
	Failed,
	NameAlreadyExists,
	NameDoesNotExist,
	AlreadyExists,
	DoesNotExist,
	CouldNotLoad,
	CouldNotParse,
	Unauthorized,
	Unavailable,
	TimedOut,
	ComputerOnFire,
	Locked,
	Encripted,
	UnknownAdress,
	InvalidData,
	InvalidChecksum,
	Overflow,
	Underflow,
	TooMuchRizz,
	SuspicusResponse,
	Unknown,
	InvalidInput,
	InvalidOutput,
	ValueDoesNotExist,
	ValueAlreadyExists,
	KeyDoesNotExist,
	KeyAlreadyExists,
	Max // meaningless.
};

extern const std::string& GetErrorName(ErrorCode error);


_R2D_NAMESPACE_END_