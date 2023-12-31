#include "global.h"
#include "Errors.h"

_R2D_NAMESPACE_START_

constexpr size_t MaxError = (size_t)ErrorCode::Max;

// no good
const std::string ErrorName[MaxError]
{
  "ok",
  "failed",
  "name already exits",
  "already exits",
  "could not load"
};

const std::string& GetErrorName(ErrorCode error)
{
  return ErrorName[(size_t)error];
}

_R2D_NAMESPACE_END_

