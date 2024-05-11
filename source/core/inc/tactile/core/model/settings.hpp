// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/i18n/language_id.hpp"

namespace tactile {

/// \addtogroup Model
/// \{

/**
 * Represents editor settings that the user can control.
 */
struct Settings final
{
  /** The language that will be loaded at startup. */
  LanguageID language;

  /** The UI font size. */
  float font_size;

  /** Whether verbose events (e.g., some mouse events) should be logged. */
  bool log_verbose_events : 1;
};

/**
 * Returns the settings used by default.
 *
 * \return
 * The default settings.
 */
[[nodiscard]]
auto get_default_settings() -> Settings;

/// \}

}  // namespace tactile
