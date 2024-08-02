// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>  // replace

#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Converts the native separators in a path to forward slashes.
 *
 * \param path The original path.
 *
 * \return
 * A string that represents the provided path but using forward slashes.
 */
[[nodiscard]]
inline auto normalize_path_separators(const Path& path) -> String
{
  auto path_str = path.string();

  if constexpr (Path::preferred_separator != '/') {
    std::ranges::replace(path_str, Path::preferred_separator, '/');
  }

  return path_str;
}

}  // namespace tactile
