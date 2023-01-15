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

#include "common/macros.hpp"
#include "common/numeric.hpp"
#include "common/type/math.hpp"
#include "common/type/ptr.hpp"
#include "common/type/string.hpp"
#include "core/color.hpp"
#include "lang/language.hpp"
#include "ui/style/themes.hpp"

namespace tactile {

/// Encodes different viewport overlay positions.
/// The enumerator values cannot change, and should mirror those of proto::OverlayPos.
enum class OverlayPos {
  TopLeft = 0,
  TopRight = 1,
  BottomLeft = 2,
  BottomRight = 3
};

using SettingsFlagBits = uint32;

enum SettingsFlags : SettingsFlagBits {
  SETTINGS_EMBED_TILESETS_BIT = 1u << 0u,
  SETTINGS_INDENT_OUTPUT_BIT = 1u << 1u,
  SETTINGS_FOLD_TILE_DATA_BIT = 1u << 2u,
  SETTINGS_SHOW_GRID_BIT = 1u << 3u,
  SETTINGS_SHOW_LAYER_DOCK_BIT = 1u << 4u,
  SETTINGS_SHOW_TILESET_DOCK_BIT = 1u << 5u,
  SETTINGS_SHOW_PROPERTY_DOCK_BIT = 1u << 6u,
  SETTINGS_SHOW_COMPONENT_DOCK_BIT = 1u << 7u,
  SETTINGS_SHOW_LOG_DOCK_BIT = 1u << 8u,
  SETTINGS_SHOW_ANIMATION_DOCK_BIT = 1u << 9u,
  SETTINGS_WINDOW_BORDER_BIT = 1u << 10u,
  SETTINGS_RESTORE_LAYOUT_BIT = 1u << 11u,
  SETTINGS_RESTORE_LAST_SESSION_BIT = 1u << 12u,
  SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT = 1u << 13u,
  SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT = 1u << 14u,
  SETTINGS_USE_DEFAULT_FONT_BIT = 1u << 15u,
};

TACTILE_FWD_DECLARE_STRUCT(SettingsState)

/// Collection of all persistent settings.
class Settings final {
 public:
  TACTILE_DELETE_COPY(Settings);
  TACTILE_DECLARE_MOVE(Settings);

  /// Creates an instance initialized with the default setting values.
  Settings();

  ~Settings();

  void print() const;

  void copy_values_from(const Settings& settings);

  void reset_values();

  void reset_behavior_values();
  void reset_appearance_values();
  void reset_export_values();
  void reset_dock_visibilities();

  void set_language(Lang lang);
  void set_theme(ui::EditorTheme theme);
  void set_viewport_overlay_pos(OverlayPos pos);
  void set_command_capacity(usize capacity);
  void set_preferred_tile_size(Int2 tile_size);
  void set_font_size(int32 size);
  void set_viewport_bg_color(Color color);
  void set_grid_color(Color color);
  void set_preferred_format(String format);

  void set_flags(SettingsFlagBits flags);
  void set_flag(SettingsFlagBits flag, bool value = true);
  void negate_flag(SettingsFlagBits flag);

  [[nodiscard]] auto get_language() const -> Lang;
  [[nodiscard]] auto get_theme() const -> ui::EditorTheme;
  [[nodiscard]] auto get_viewport_overlay_pos() const -> OverlayPos;
  [[nodiscard]] auto get_command_capacity() const -> usize;
  [[nodiscard]] auto get_preferred_tile_size() const -> Int2;
  [[nodiscard]] auto get_font_size() const -> int32;
  [[nodiscard]] auto get_viewport_bg_color() const -> const Color&;
  [[nodiscard]] auto get_grid_color() const -> const Color&;
  [[nodiscard]] auto get_preferred_format() const -> const String&;

  [[nodiscard]] auto get_flags() const -> SettingsFlagBits;
  [[nodiscard]] auto test_flag(SettingsFlagBits flag) const -> bool;

 private:
  Unique<SettingsState> mState;

  void reset_flag(SettingsFlagBits flag);
};

[[nodiscard]] auto get_settings() -> Settings&;

}  // namespace tactile
