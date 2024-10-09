// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional
#include <string>    // string

#include "tactile/base/prelude.hpp"

namespace tactile::core {

/**
 * Reads the value of an environment variable.
 *
 * \param name The environment variable name, can safely be null.
 *
 * \return
 * The environment variable value if successful; an empty optional otherwise.
 */
[[nodiscard]]
auto get_env(const char* name) -> std::optional<std::string>;

}  // namespace tactile::core
