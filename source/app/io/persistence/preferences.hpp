/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>  // string

#include <centurion/color.hpp>

#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/themes.hpp"

namespace tactile::io {

/// Encodes different viewport overlay positions.
/// The enumerator values cannot change, and should mirror those of proto::OverlayPos.
enum class OverlayPos
{
  TopLeft = 0,
  TopRight = 1,
  BottomLeft = 2,
  BottomRight = 3
};

inline constexpr auto       def_preferred_format = "YAML";
inline constexpr auto       def_viewport_overlay_pos = OverlayPos::BottomRight;
inline constexpr auto       def_theme = ui::EditorTheme::Nocturnal;
inline constexpr cen::color def_viewport_bg {0x3C, 0x3C, 0x3C};
inline constexpr Vector2i   def_preferred_tile_size {32, 32};
inline constexpr usize      def_command_capacity = 100;

inline constexpr bool def_embed_tilesets = false;
inline constexpr bool def_indent_output = true;
inline constexpr bool def_fold_tile_data = false;
inline constexpr bool def_show_grid = true;
inline constexpr bool def_show_layer_dock = true;
inline constexpr bool def_show_tileset_dock = true;
inline constexpr bool def_show_property_dock = true;
inline constexpr bool def_show_component_dock = true;
inline constexpr bool def_show_log_dock = false;
inline constexpr bool def_window_border = false;
inline constexpr bool def_restore_layout = true;
inline constexpr bool def_restore_last_session = true;
inline constexpr bool def_show_viewport_overlay_fps = true;
inline constexpr bool def_highlight_active_layer = false;
inline constexpr bool def_use_default_font = true;

/// Contains all persistent settings.
/// Note, the boolean members cannot be bitfields because we take the address of them.
struct PreferenceState final
{
  std::string     preferred_format {def_preferred_format};
  ui::EditorTheme theme {def_theme};
  cen::color      viewport_background {def_viewport_bg};
  usize           command_capacity {def_command_capacity};
  Vector2i        preferred_tile_size {def_preferred_tile_size};
  int32           font_size {ui::def_font_size};
  OverlayPos      viewport_overlay_pos {def_viewport_overlay_pos};
  bool            embed_tilesets {def_embed_tilesets};
  bool            indent_output {def_indent_output};
  bool            fold_tile_data {def_fold_tile_data};
  bool            show_grid {def_show_grid};
  bool            show_layer_dock {def_show_layer_dock};
  bool            show_tileset_dock {def_show_tileset_dock};
  bool            show_property_dock {def_show_property_dock};
  bool            show_component_dock {def_show_component_dock};
  bool            show_log_dock {def_show_log_dock};
  bool            window_border {def_window_border};
  bool            restore_layout {def_restore_layout};
  bool            restore_last_session {def_restore_last_session};
  bool            show_viewport_overlay_fps {def_show_viewport_overlay_fps};
  bool            highlight_active_layer {def_highlight_active_layer};
  bool            use_default_font {def_use_default_font};
};

/// Loads any existing persistent preferences, or assumes the default ones.
void load_preferences();

/// Saves the current preferences in the persistent file directory.
void save_preferences();

void print_preferences();

/// Overrides the current in-memory preferences.
void set_preferences(PreferenceState prefs);

/// Returns the current in-memory preferences.
[[nodiscard]] auto get_preferences() -> PreferenceState&;

}  // namespace tactile::io
