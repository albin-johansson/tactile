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

#include "preference_state.hpp"

#include <fstream>  // ifstream, ofstream
#include <ios>      // ios
#include <utility>  // move

#include <magic_enum.hpp>

#include "misc/logging.hpp"
#include "proto.hpp"

namespace tactile {
namespace {

constexpr EditorTheme _def_theme = EditorTheme::nocturnal;
constexpr cen::color _def_viewport_bg{60, 60, 60};

constexpr auto _def_preferred_format = "YAML";

constexpr usize _def_command_capacity = 100;
constexpr int32 _def_preferred_tile_width = 32;
constexpr int32 _def_preferred_tile_height = 32;
constexpr int32 _def_viewport_overlay_pos = cen::to_underlying(OverlayPos::bottom_left);
constexpr int32 _def_font_size = 14;

constexpr uint64 _bit_embed_tilesets = 1u << 0u;
constexpr uint64 _bit_indent_output = 1u << 1u;
constexpr uint64 _bit_fold_tile_data = 1u << 2u;
constexpr uint64 _bit_show_grid = 1u << 3u;
constexpr uint64 _bit_show_layer_dock = 1u << 4u;
constexpr uint64 _bit_show_tileset_dock = 1u << 5u;
constexpr uint64 _bit_show_property_dock = 1u << 6u;
constexpr uint64 _bit_show_log_dock = 1u << 7u;
constexpr uint64 _bit_window_border = 1u << 8u;
constexpr uint64 _bit_restore_layout = 1u << 9u;
constexpr uint64 _bit_restore_last_session = 1u << 10u;
constexpr uint64 _bit_show_component_dock = 1u << 11u;
constexpr uint64 _bit_show_viewport_overlay_fps = 1u << 12u;
constexpr uint64 _bit_highlight_active_layer = 1u << 13u;
constexpr uint64 _bit_use_default_font = 1u << 14u;

constexpr uint64 _def_flags = _bit_show_grid |                  //
                              _bit_indent_output |              //
                              _bit_show_layer_dock |            //
                              _bit_show_tileset_dock |          //
                              _bit_show_property_dock |         //
                              _bit_show_component_dock |        //
                              _bit_restore_layout |             //
                              _bit_restore_last_session |       //
                              _bit_show_viewport_overlay_fps |  //
                              _bit_use_default_font;

}  // namespace

#define PRINT_FLAG(Name, Mask) \
  log_debug(Name "... {}", (mData->flags & (Mask)) ? "yes" : "no")

struct PreferenceState::Data
{
  std::string preferred_format{_def_preferred_format};

  EditorTheme theme{_def_theme};
  cen::color viewport_background{_def_viewport_bg};

  usize command_capacity{_def_command_capacity};

  int32 preferred_tile_width{_def_preferred_tile_width};
  int32 preferred_tile_height{_def_preferred_tile_height};
  int32 viewport_overlay_pos{_def_viewport_overlay_pos};
  int32 font_size{_def_font_size};

  uint64 flags{_def_flags};
};

PreferenceState::PreferenceState() : mData{std::make_unique<Data>()} {}

PreferenceState::PreferenceState(const PreferenceState& other)
    : mData{std::make_unique<Data>()}
{
  *mData = *other.mData;
}

PreferenceState::PreferenceState(PreferenceState&&) noexcept = default;

auto PreferenceState::operator=(const PreferenceState& other) -> PreferenceState&
{
  if (this != &other) {
    mData = std::make_unique<Data>();
    *mData = *other.mData;
  }
  return *this;
}

auto PreferenceState::operator=(PreferenceState&&) noexcept -> PreferenceState& = default;

PreferenceState::~PreferenceState() noexcept = default;

void PreferenceState::print()
{
  log_debug("Theme... {}", magic_enum::enum_name(mData->theme));
  log_debug("Viewport background... {}", mData->viewport_background.as_rgb());

  log_debug("Command capacity... {}", mData->command_capacity);
  log_debug("Preferred tile width... {}", mData->preferred_tile_width);
  log_debug("Preferred tile height... {}", mData->preferred_tile_height);

  log_debug("Preferred format... {}", mData->preferred_format);
  log_debug("Viewport overlay pos... {}", mData->viewport_overlay_pos);
  PRINT_FLAG("Show FPS in viewport overlay", _bit_show_viewport_overlay_fps);

  log_debug("Font size... {}", mData->font_size);
  PRINT_FLAG("Use default font", _bit_use_default_font);

  PRINT_FLAG("Embed tilesets", _bit_embed_tilesets);
  PRINT_FLAG("Indent output", _bit_indent_output);
  PRINT_FLAG("Fold tile data", _bit_fold_tile_data);

  PRINT_FLAG("Show grid", _bit_show_grid);
  PRINT_FLAG("Highlight active layer", _bit_highlight_active_layer);
  PRINT_FLAG("Show layer dock", _bit_show_layer_dock);
  PRINT_FLAG("Show log dock", _bit_show_log_dock);
  PRINT_FLAG("Show tileset dock", _bit_show_tileset_dock);
  PRINT_FLAG("Show property dock", _bit_show_property_dock);
  PRINT_FLAG("Show component dock", _bit_show_component_dock);

  PRINT_FLAG("Window border", _bit_window_border);
  PRINT_FLAG("Restore layout", _bit_restore_layout);
  PRINT_FLAG("Restore last session", _bit_restore_last_session);
}

void PreferenceState::parse(const std::filesystem::path& path)
{
  std::ifstream stream{path, std::ios::in | std::ios::binary};

  proto::Settings cfg;
  if (cfg.ParseFromIstream(&stream)) {
    if (cfg.has_theme()) {
      mData->theme = static_cast<EditorTheme>(cfg.theme());
    }

    if (cfg.has_viewport_background()) {
      const auto& color = cfg.viewport_background();
      mData->viewport_background.set_red(static_cast<uint8>(color.red()));
      mData->viewport_background.set_green(static_cast<uint8>(color.green()));
      mData->viewport_background.set_blue(static_cast<uint8>(color.blue()));
      mData->viewport_background.set_alpha(static_cast<uint8>(color.alpha()));
    }

    if (cfg.has_show_grid()) {
      set_flag(_bit_show_grid, cfg.show_grid());
    }

    if (cfg.has_highlight_active_layer()) {
      set_flag(_bit_highlight_active_layer, cfg.highlight_active_layer());
    }

    if (cfg.has_window_border()) {
      set_flag(_bit_window_border, cfg.window_border());
    }

    if (cfg.has_command_capacity()) {
      mData->command_capacity = cfg.command_capacity();
    }

    if (cfg.has_restore_last_session()) {
      set_flag(_bit_restore_last_session, cfg.restore_last_session());
    }

    if (cfg.has_preferred_tile_width()) {
      mData->preferred_tile_width = cfg.preferred_tile_width();
    }

    if (cfg.has_preferred_tile_height()) {
      mData->preferred_tile_height = cfg.preferred_tile_height();
    }

    if (cfg.has_preferred_format()) {
      mData->preferred_format = cfg.preferred_format();
    }

    if (cfg.has_embed_tilesets()) {
      set_flag(_bit_embed_tilesets, cfg.embed_tilesets());
    }

    if (cfg.has_indent_output()) {
      set_flag(_bit_indent_output, cfg.indent_output());
    }

    if (cfg.has_fold_tile_data()) {
      set_flag(_bit_fold_tile_data, cfg.fold_tile_data());
    }

    if (cfg.has_show_tileset_dock()) {
      set_flag(_bit_show_tileset_dock, cfg.show_tileset_dock());
    }

    if (cfg.has_show_layer_dock()) {
      set_flag(_bit_show_layer_dock, cfg.show_layer_dock());
    }

    if (cfg.has_show_property_dock()) {
      set_flag(_bit_show_property_dock, cfg.show_property_dock());
    }

    if (cfg.has_show_log_dock()) {
      set_flag(_bit_show_log_dock, cfg.show_log_dock());
    }

    if (cfg.has_show_component_dock()) {
      set_flag(_bit_show_component_dock, cfg.show_component_dock());
    }

    if (cfg.has_restore_layout()) {
      set_flag(_bit_restore_layout, cfg.restore_layout());
    }

    if (cfg.has_viewport_overlay_pos()) {
      mData->viewport_overlay_pos = cfg.viewport_overlay_pos();
    }

    if (cfg.has_viewport_overlay_show_fps()) {
      set_flag(_bit_show_viewport_overlay_fps, cfg.viewport_overlay_show_fps());
    }

    if (cfg.has_use_default_font()) {
      set_flag(_bit_use_default_font, cfg.use_default_font());
    }

    if (cfg.has_font_size()) {
      mData->font_size = cfg.font_size();
    }
  }
}

void PreferenceState::save(const std::filesystem::path& path)
{
  proto::Settings cfg;

  cfg.set_theme(static_cast<proto::Theme>(mData->theme));
  cfg.set_show_grid(is_grid_visible());
  cfg.set_highlight_active_layer(highlight_active_layer());
  cfg.set_window_border(has_window_border());

  {
    const auto& bg = viewport_bg();
    auto* background = cfg.mutable_viewport_background();
    background->set_red(bg.red());
    background->set_green(bg.green());
    background->set_blue(bg.blue());
    background->set_alpha(bg.alpha());
  }

  cfg.set_command_capacity(command_capacity());
  cfg.set_restore_last_session(will_restore_last_session());
  cfg.set_preferred_tile_width(preferred_tile_width());
  cfg.set_preferred_tile_height(preferred_tile_height());

  cfg.set_preferred_format(preferred_format());
  cfg.set_embed_tilesets(embed_tilesets());
  cfg.set_indent_output(indent_output());
  cfg.set_fold_tile_data(fold_tile_data());

  cfg.set_show_tileset_dock(is_tileset_dock_visible());
  cfg.set_show_layer_dock(is_layer_dock_visible());
  cfg.set_show_property_dock(is_property_dock_visible());
  cfg.set_show_log_dock(is_log_dock_visible());
  cfg.set_show_component_dock(is_component_dock_visible());
  cfg.set_restore_layout(will_restore_layout());
  cfg.set_viewport_overlay_pos(
      proto::OverlayPos{cen::to_underlying(viewport_overlay_pos())});
  cfg.set_viewport_overlay_show_fps(viewport_overlay_show_fps());

  cfg.set_use_default_font(use_default_font());
  cfg.set_font_size(font_size());

  std::ofstream stream{path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    log_error("Failed to save preferences!");
  }
}

void PreferenceState::reset_appearance_preferences()
{
  mData->theme = _def_theme;
  mData->viewport_background = _def_viewport_bg;

  reset_flag(_bit_window_border);
  reset_flag(_bit_show_grid);

  /* Note, not technically an appearance setting, but it is to the user */
  reset_flag(_bit_restore_layout);

  reset_flag(_bit_use_default_font);
  mData->font_size = _def_font_size;
}

void PreferenceState::reset_behavior_preferences()
{
  mData->command_capacity = _def_command_capacity;
  mData->preferred_tile_width = _def_preferred_tile_width;
  mData->preferred_tile_height = _def_preferred_tile_height;
  reset_flag(_bit_restore_last_session);
}

void PreferenceState::reset_export_preferences()
{
  mData->preferred_format = _def_preferred_format;
  reset_flag(_bit_embed_tilesets);
  reset_flag(_bit_indent_output);
  reset_flag(_bit_fold_tile_data);
}

void PreferenceState::reset_dock_visibilities()
{
  reset_flag(_bit_show_layer_dock);
  reset_flag(_bit_show_tileset_dock);
  reset_flag(_bit_show_property_dock);
  reset_flag(_bit_show_component_dock);
  reset_flag(_bit_show_log_dock);
}

void PreferenceState::set_theme(const EditorTheme theme)
{
  mData->theme = theme;
}

auto PreferenceState::get_theme() const -> EditorTheme
{
  return mData->theme;
}

void PreferenceState::set_viewport_bg(const cen::color& bg)
{
  mData->viewport_background = bg;
}

auto PreferenceState::viewport_bg() const -> const cen::color&
{
  return mData->viewport_background;
}

void PreferenceState::set_window_border(const bool enable)
{
  set_flag(_bit_window_border, enable);
}

auto PreferenceState::has_window_border() const -> bool
{
  return test_flag(_bit_window_border);
}

void PreferenceState::set_layer_dock_visible(const bool visible)
{
  set_flag(_bit_show_layer_dock, visible);
}

auto PreferenceState::is_layer_dock_visible() const -> bool
{
  return test_flag(_bit_show_layer_dock);
}

void PreferenceState::set_tileset_dock_visible(const bool visible)
{
  set_flag(_bit_show_tileset_dock, visible);
}

auto PreferenceState::is_tileset_dock_visible() const -> bool
{
  return test_flag(_bit_show_tileset_dock);
}

void PreferenceState::set_property_dock_visible(const bool visible)
{
  set_flag(_bit_show_property_dock, visible);
}

auto PreferenceState::is_property_dock_visible() const -> bool
{
  return test_flag(_bit_show_property_dock);
}

void PreferenceState::set_component_dock_visible(const bool visible)
{
  set_flag(_bit_show_component_dock, visible);
}

auto PreferenceState::is_component_dock_visible() const -> bool
{
  return test_flag(_bit_show_component_dock);
}

void PreferenceState::set_log_dock_visible(const bool visible)
{
  set_flag(_bit_show_log_dock, visible);
}

auto PreferenceState::is_log_dock_visible() const -> bool
{
  return test_flag(_bit_show_log_dock);
}

void PreferenceState::set_grid_visible(const bool visible)
{
  set_flag(_bit_show_grid, visible);
}

auto PreferenceState::is_grid_visible() const -> bool
{
  return test_flag(_bit_show_grid);
}

void PreferenceState::set_highlight_active_layer(const bool highlight)
{
  set_flag(_bit_highlight_active_layer, highlight);
}

auto PreferenceState::highlight_active_layer() const -> bool
{
  return test_flag(_bit_highlight_active_layer);
}

void PreferenceState::set_embed_tilesets(const bool embed)
{
  set_flag(_bit_embed_tilesets, embed);
}

auto PreferenceState::embed_tilesets() const -> bool
{
  return test_flag(_bit_embed_tilesets);
}

void PreferenceState::set_fold_tile_data(const bool fold)
{
  set_flag(_bit_fold_tile_data, fold);
}

auto PreferenceState::fold_tile_data() const -> bool
{
  return test_flag(_bit_fold_tile_data);
}

void PreferenceState::set_indent_output(const bool indent)
{
  set_flag(_bit_indent_output, indent);
}

auto PreferenceState::indent_output() const -> bool
{
  return test_flag(_bit_indent_output);
}

void PreferenceState::set_will_restore_layout(const bool restore)
{
  set_flag(_bit_restore_layout, restore);
}

auto PreferenceState::will_restore_layout() const -> bool
{
  return test_flag(_bit_restore_layout);
}

void PreferenceState::set_will_restore_last_session(const bool restore)
{
  set_flag(_bit_restore_last_session, restore);
}

auto PreferenceState::will_restore_last_session() const -> bool
{
  return test_flag(_bit_restore_last_session);
}

void PreferenceState::set_viewport_overlay_pos(const OverlayPos pos)
{
  mData->viewport_overlay_pos = cen::to_underlying(pos);
}

auto PreferenceState::viewport_overlay_pos() const -> OverlayPos
{
  return static_cast<OverlayPos>(mData->viewport_overlay_pos);
}

void PreferenceState::set_viewport_overlay_show_fps(const bool show)
{
  set_flag(_bit_show_viewport_overlay_fps, show);
}

auto PreferenceState::viewport_overlay_show_fps() const -> bool
{
  return test_flag(_bit_show_viewport_overlay_fps);
}

void PreferenceState::set_command_capacity(const usize capacity)
{
  mData->command_capacity = capacity;
}

auto PreferenceState::command_capacity() const -> usize
{
  return mData->command_capacity;
}

void PreferenceState::set_preferred_format(std::string format)
{
  mData->preferred_format = std::move(format);
}

auto PreferenceState::preferred_format() const -> const std::string&
{
  return mData->preferred_format;
}

void PreferenceState::set_preferred_tile_width(const int32 width)
{
  mData->preferred_tile_width = width;
}

auto PreferenceState::preferred_tile_width() const -> int32
{
  return mData->preferred_tile_width;
}

void PreferenceState::set_preferred_tile_height(const int32 height)
{
  mData->preferred_tile_height = height;
}

auto PreferenceState::preferred_tile_height() const -> int32
{
  return mData->preferred_tile_height;
}

void PreferenceState::set_font_size(const int32 size)
{
  mData->font_size = size;
}

auto PreferenceState::font_size() const -> int32
{
  return mData->font_size;
}

void PreferenceState::set_use_default_font(const bool use)
{
  set_flag(_bit_use_default_font, use);
}

auto PreferenceState::use_default_font() const -> bool
{
  return test_flag(_bit_use_default_font);
}

void PreferenceState::set_flag(const uint64 flag, const bool value) noexcept
{
  if (value) {
    mData->flags |= flag;
  }
  else {
    mData->flags &= ~flag;
  }
}

void PreferenceState::reset_flag(const uint64 flag) noexcept
{
  set_flag(flag, _def_flags & flag);
}

auto PreferenceState::test_flag(const uint64 flag) const noexcept -> bool
{
  return mData->flags & flag;
}

}  // namespace tactile