// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/persist/settings.hpp"

namespace tactile {

auto default_settings() -> Settings
{
  return {
    .language = ui::LanguageID::kAmericanEnglish,
    .preferred_save_format_extension = ".yml",
    .command_capacity = 100,
    .font_size = 13,
    .theme_saturation = 60,
    .preferred_tile_size = {32, 32},
    .viewport_background_color = {0x3C, 0x3C, 0x3C, 0xFF},
    .grid_line_color = {0xFF, 0xFF, 0xFF, 0x05},
  };
}

void reset_settings(Settings& settings)
{
  settings = default_settings();
}

}  // namespace tactile
