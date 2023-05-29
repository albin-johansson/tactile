/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "settings.hpp"

#include <utility>  // move

#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "common/fmt/lang_formatter.hpp"
#include "common/fmt/save_format_formatter.hpp"
#include "common/fmt/theme_formatter.hpp"
#include "ui/constants.hpp"

namespace tactile {

struct SettingsState final {
  Lang language {Lang::EN};
  Theme theme {Theme::Nocturnal};
  OverlayPos viewport_overlay_pos {OverlayPos::BottomRight};

  usize command_capacity {100};
  Int2 preferred_tile_size {32, 32};
  int32 font_size {kDefFontSize};
  int32 theme_saturation {60};

  Color viewport_bg {0x3C, 0x3C, 0x3C};
  Color grid_color {0xFF, 0xFF, 0xFF, 0x05};

  SaveFormat preferred_format {SaveFormat::TactileYaml};

  SettingsFlagBits flags {
      SETTINGS_INDENT_OUTPUT_BIT | SETTINGS_SHOW_GRID_BIT | SETTINGS_SHOW_LAYER_DOCK_BIT |
      SETTINGS_SHOW_TILESET_DOCK_BIT | SETTINGS_SHOW_PROPERTY_DOCK_BIT |
      SETTINGS_SHOW_COMPONENT_DOCK_BIT | SETTINGS_SHOW_ANIMATION_DOCK_BIT |
      SETTINGS_RESTORE_LAYOUT_BIT | SETTINGS_RESTORE_LAST_SESSION_BIT |
      SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT | SETTINGS_USE_DEFAULT_FONT_BIT};
};

// Used to be able to easily reset settings.
inline static const SettingsState kDefaultSettings = SettingsState {};

Settings::Settings()
    : mState {std::make_unique<SettingsState>()}
{
}

Settings::~Settings() = default;

TACTILE_DEFINE_MOVE(Settings);

void Settings::print() const
{
  spdlog::debug("[Settings] Language: {}", get_language());
  spdlog::debug("[Settings] Theme: {}", get_theme());
  spdlog::debug("[Settings] Theme saturation: {}", get_theme_saturation());
  spdlog::debug("[Settings] Viewport background: {}", get_viewport_bg_color().as_rgb());
  spdlog::debug("[Settings] Grid color: {}", get_grid_color().as_rgba());

  spdlog::debug("[Settings] Command capacity: {}", get_command_capacity());
  spdlog::debug("[Settings] Preferred tile width: {}", get_preferred_tile_size().x);
  spdlog::debug("[Settings] Preferred tile height: {}", get_preferred_tile_size().y);

  spdlog::debug("[Settings] Preferred format: {}", get_preferred_format());
  spdlog::debug("[Settings] Viewport overlay pos: {}",
                magic_enum::enum_name(get_viewport_overlay_pos()));
  spdlog::debug("[Settings] Show FPS in viewport overlay: {}",
                test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT));

  spdlog::debug("[Settings] Font size: {}", get_font_size());
  spdlog::debug("[Settings] Use default font: {}",
                test_flag(SETTINGS_USE_DEFAULT_FONT_BIT));

  spdlog::debug("[Settings] Embed tilesets: {}", test_flag(SETTINGS_EMBED_TILESETS_BIT));
  spdlog::debug("[Settings] Indent output: {}", test_flag(SETTINGS_INDENT_OUTPUT_BIT));
  spdlog::debug("[Settings] Fold tile data: {}", test_flag(SETTINGS_FOLD_TILE_DATA_BIT));

  spdlog::debug("[Settings] Show grid: {}", test_flag(SETTINGS_SHOW_GRID_BIT));
  spdlog::debug("[Settings] Highlight active layer: {}",
                test_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT));
  spdlog::debug("[Settings] Show layer dock: {}",
                test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT));
  spdlog::debug("[Settings] Show log dock: {}", test_flag(SETTINGS_SHOW_LOG_DOCK_BIT));
  spdlog::debug("[Settings] Show tileset dock: {}",
                test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT));
  spdlog::debug("[Settings] Show property dock: {}",
                test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT));
  spdlog::debug("[Settings] Show component dock: {}",
                test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT));
  spdlog::debug("[Settings] Show animation dock: {}",
                test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT));

  spdlog::debug("[Settings] Window border: {}", test_flag(SETTINGS_WINDOW_BORDER_BIT));
  spdlog::debug("[Settings] Restore layout: {}", test_flag(SETTINGS_RESTORE_LAYOUT_BIT));
  spdlog::debug("[Settings] Restore last session: {}",
                test_flag(SETTINGS_RESTORE_LAST_SESSION_BIT));
}

auto Settings::copy() const -> Settings
{
  Settings result;
  result.copy_from(*this);
  return result;
}

void Settings::copy_from(const Settings& settings)
{
  *mState = *settings.mState;
}

void Settings::reset_values()
{
  *mState = kDefaultSettings;
}

void Settings::reset_behavior_values()
{
  mState->command_capacity = kDefaultSettings.command_capacity;
  mState->preferred_tile_size = kDefaultSettings.preferred_tile_size;

  reset_flag(SETTINGS_RESTORE_LAST_SESSION_BIT);
}

void Settings::reset_appearance_values()
{
  mState->language = kDefaultSettings.language;
  mState->theme = kDefaultSettings.theme;
  mState->theme_saturation = kDefaultSettings.theme_saturation;
  mState->viewport_bg = kDefaultSettings.viewport_bg;
  mState->grid_color = kDefaultSettings.grid_color;
  mState->font_size = kDefaultSettings.font_size;

  reset_flag(SETTINGS_WINDOW_BORDER_BIT);
  reset_flag(SETTINGS_SHOW_GRID_BIT);
  reset_flag(SETTINGS_RESTORE_LAYOUT_BIT);
  reset_flag(SETTINGS_USE_DEFAULT_FONT_BIT);
}

void Settings::reset_export_values()
{
  mState->preferred_format = kDefaultSettings.preferred_format;

  reset_flag(SETTINGS_EMBED_TILESETS_BIT);
  reset_flag(SETTINGS_INDENT_OUTPUT_BIT);
  reset_flag(SETTINGS_FOLD_TILE_DATA_BIT);
}

void Settings::reset_dock_visibilities()
{
  reset_flag(SETTINGS_SHOW_LAYER_DOCK_BIT);
  reset_flag(SETTINGS_SHOW_TILESET_DOCK_BIT);
  reset_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT);
  reset_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT);
  reset_flag(SETTINGS_SHOW_LOG_DOCK_BIT);
  reset_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT);
}

void Settings::set_language(const Lang lang)
{
  mState->language = lang;
}

void Settings::set_theme(const Theme theme)
{
  mState->theme = theme;
}

void Settings::set_theme_saturation(const int32 saturation)
{
  mState->theme_saturation = saturation;
}

void Settings::set_viewport_overlay_pos(const OverlayPos pos)
{
  mState->viewport_overlay_pos = pos;
}

void Settings::set_command_capacity(const usize capacity)
{
  mState->command_capacity = capacity;
}

void Settings::set_preferred_tile_size(const Int2 tile_size)
{
  mState->preferred_tile_size = tile_size;
}

void Settings::set_font_size(const int32 size)
{
  mState->font_size = size;
}

void Settings::set_viewport_bg_color(const Color color)
{
  mState->viewport_bg = color;
}

void Settings::set_grid_color(const Color color)
{
  mState->grid_color = color;
}

void Settings::set_preferred_format(const SaveFormat format)
{
  mState->preferred_format = format;
}

void Settings::set_flags(const SettingsFlagBits flags)
{
  mState->flags = flags;
}

auto Settings::get_language() const -> Lang
{
  return mState->language;
}

auto Settings::get_theme() const -> Theme
{
  return mState->theme;
}

auto Settings::get_theme_saturation() const -> int32
{
  return mState->theme_saturation;
}

auto Settings::get_viewport_overlay_pos() const -> OverlayPos
{
  return mState->viewport_overlay_pos;
}

auto Settings::get_command_capacity() const -> usize
{
  return mState->command_capacity;
}

auto Settings::get_preferred_tile_size() const -> Int2
{
  return mState->preferred_tile_size;
}

auto Settings::get_font_size() const -> int32
{
  return mState->font_size;
}

auto Settings::get_viewport_bg_color() const -> const Color&
{
  return mState->viewport_bg;
}

auto Settings::get_grid_color() const -> const Color&
{
  return mState->grid_color;
}

auto Settings::get_preferred_format() const -> const SaveFormat&
{
  return mState->preferred_format;
}

auto Settings::get_flags() const -> SettingsFlagBits
{
  return mState->flags;
}

void Settings::reset_flag(const SettingsFlagBits flag)
{
  set_flag(flag, kDefaultSettings.flags & flag);
}

void Settings::set_flag(const SettingsFlagBits flag, const bool value)
{
  if (value) {
    mState->flags |= flag;
  }
  else {
    mState->flags &= ~flag;
  }
}

void Settings::negate_flag(const SettingsFlagBits flag)
{
  set_flag(flag, !test_flag(flag));
}

auto Settings::test_flag(const SettingsFlagBits flag) const -> bool
{
  return mState->flags & flag;
}

}  // namespace tactile