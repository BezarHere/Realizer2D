#include "global.h"
#include "Errors.h"

_R2D_NAMESPACE_START_

constexpr size_t MaxError = (size_t)Error::Max;

// no good
const std::string ErrorName[MaxError]
{
  "ok",
  "failed",
  "name already exits",
  "already exits"
};

const std::string& GetErrorName(Error error)
{
  return ErrorName[(size_t)error];
}

_R2D_NAMESPACE_END_

