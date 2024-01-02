// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/save/save_format_id.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/meta/color.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents the persistent editor settings.
 */
struct Settings final {
  ui::LanguageID language {ui::LanguageID::kAmericanEnglish};
  SaveFormatId preferred_save_format {SaveFormatId::kTactileYaml};
  usize command_capacity {100};
  int32 font_size {13};
  int32 theme_saturation {60};
  Int2 preferred_tile_size {32, 32};
  UColor viewport_background_color {0x3C, 0x3C, 0x3C, 0xFF};
  UColor grid_line_color {0xFF, 0xFF, 0xFF, 0x05};
  bool use_default_font : 1 {true};
  bool embed_tilesets : 1 {true};
  bool indent_output : 1 {true};
  bool fold_tile_data : 1 {true};
  bool show_active_layer_highlight : 1 {false};
  bool show_layer_dock : 1 {true};
  bool show_tileset_dock : 1 {true};
  bool show_property_dock : 1 {true};
  bool show_component_dock : 1 {true};
  bool show_animation_dock : 1 {true};
  bool show_log_dock : 1 {false};
  bool show_viewport_grid : 1 {true};
  bool show_viewport_overlay_fps : 1 {true};
  bool show_window_borders : 1 {false};
  bool restore_layout_at_startup : 1 {false};
  bool restore_session_at_startup : 1 {true};
};

}  // namespace tactile
