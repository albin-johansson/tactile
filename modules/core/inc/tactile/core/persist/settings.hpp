// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/meta/color.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents the persistent editor settings.
 */
struct Settings final {
  ui::LanguageID language;
  String preferred_save_format_extension;
  usize command_capacity;
  int32 font_size;
  int32 theme_saturation;
  Int2 preferred_tile_size;
  UColor viewport_background_color;
  UColor grid_line_color;
};

/**
 * \brief Creates a setting bundle with the default configuration.
 *
 * \return a settings bundle.
 */
[[nodiscard]]
TACTILE_CORE_API auto default_settings() -> Settings;

/**
 * \brief Resets a setting bundle.
 *
 * \param settings the target settings.
 */
TACTILE_CORE_API void reset_settings(Settings& settings);

}  // namespace tactile
