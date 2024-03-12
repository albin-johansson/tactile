// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(Strings)

/// All of the supported languages.
enum class Lang {
  EN = 0,     ///< American English
  EN_GB = 1,  ///< British English
  SV = 2      ///< Swedish
};

/// Loads all supported translation files.
void load_languages();

[[nodiscard]] auto get_language(Lang lang) -> const Strings&;

[[nodiscard]] auto get_current_language() -> const Strings&;

[[nodiscard]] auto get_language_name(Lang lang) -> const char*;

}  // namespace tactile
