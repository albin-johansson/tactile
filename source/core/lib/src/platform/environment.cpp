// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/environment.hpp"

#include <cstdlib>  // getenv, _dupenv_s, free

#if TACTILE_OS_WINDOWS
  #include <windows.h>
#endif

#include "tactile/core/util/scope_guard.hpp"

namespace tactile::core {

auto get_env(const char* name) -> std::optional<std::string>
{
  if (name) {
#if TACTILE_OS_WINDOWS
    char* value {};
    ::_dupenv_s(&value, nullptr, name);

    if (value) {
      const ScopeGuard value_deleter {[=] { std::free(value); }};
      return std::string {value};
    }
#else
    if (const auto* value = std::getenv(name)) {
      return std::string {value};
    }
#endif  // TACTILE_OS_WINDOWS
  }

  return std::nullopt;
}

}  // namespace tactile::core
