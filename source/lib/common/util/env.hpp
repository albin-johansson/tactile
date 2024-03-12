// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {

/// Returns the value of an environment variable.
[[nodiscard]] auto env_var(const char* var) -> Maybe<String>;

}  // namespace tactile
