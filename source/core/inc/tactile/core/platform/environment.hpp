// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Reads the value of an environment variable.
 *
 * \param name The environment variable name, can safely be null.
 *
 * \return
 * The environment variable value; or an error code if something went wrong.
 */
[[nodiscard]]
auto get_env(const char* name) -> std::expected<String, std::error_code>;

}  // namespace tactile
