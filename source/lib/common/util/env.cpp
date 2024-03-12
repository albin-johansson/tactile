// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "env.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#include "tactile/base/prelude.hpp"

namespace tactile {

auto env_var(const char* var) -> Maybe<String>
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

  return nothing;
}

}  // namespace tactile
