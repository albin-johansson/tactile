// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

/** Returns the value of an environment variable. */
[[nodiscard]] TACTILE_CORE_API auto get_env(const char* var) -> Maybe<String>;

}  // namespace tactile
