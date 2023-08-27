// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/environment.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#include "tactile/core/prelude.hpp"

namespace tactile {

auto get_env(const char* var) -> Maybe<String>
{
  if (var) {
#if TACTILE_OS_WINDOWS
    char* temp {};
    _dupenv_s(&temp, nullptr, var);

    if (temp) {
      String result {temp};
      std::free(temp);
      return result;
    }
#else
    if (const auto* value = std::getenv(var)) {
      return String {value};
    }
#endif  // TACTILE_OS_WINDOWS
  }

  return kNone;
}

}  // namespace tactile
