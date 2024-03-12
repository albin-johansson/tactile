// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "lang/strings.hpp"

namespace tactile {

/// Parses a language translation JSON file.
[[nodiscard]] auto parse_language(const char* path, const Strings& fallback = {})
    -> Strings;

}  // namespace tactile
