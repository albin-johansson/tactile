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

#include <filesystem>   // path
#include <string>       // string
#include <string_view>  // string_view

#include <centurion/fwd.hpp>

#include "core/common/memory.hpp"
#include "editor/gui/themes.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Encodes different viewport overlay positions.
 *
 * \warning Do not change the enumerator values!
 *
 * \see proto::OverlayPos
 */
enum class OverlayPos
{
  TopLeft = 0,
  TopRight = 1,
  BottomLeft = 2,
  BottomRight = 3
};

class PreferenceState final
{
 public:
  PreferenceState();

  PreferenceState(const PreferenceState& other);

  PreferenceState(PreferenceState&& other) noexcept;

  auto operator=(const PreferenceState& other) -> PreferenceState&;

  auto operator=(PreferenceState&& other) noexcept -> PreferenceState&;

  ~PreferenceState() noexcept;

  void parse(const std::filesystem::path& path);

  void save(const std::filesystem::path& path);

  void print();

  void reset_appearance_preferences();

  void reset_behavior_preferences();

  void reset_export_preferences();

  void reset_dock_visibilities();

  void set_theme(EditorTheme theme);
  [[nodiscard]] auto get_theme() const -> EditorTheme;

  void set_viewport_bg(const cen::color& bg);
  [[nodiscard]] auto viewport_bg() const -> const cen::color&;

  void set_window_border(bool enable);
  [[nodiscard]] auto has_window_border() const -> bool;

  void set_layer_dock_visible(bool visible);
  [[nodiscard]] auto is_layer_dock_visible() const -> bool;

  void set_tileset_dock_visible(bool visible);
  [[nodiscard]] auto is_tileset_dock_visible() const -> bool;

  void set_property_dock_visible(bool visible);
  [[nodiscard]] auto is_property_dock_visible() const -> bool;

  void set_component_dock_visible(bool visible);
  [[nodiscard]] auto is_component_dock_visible() const -> bool;

  void set_log_dock_visible(bool visible);
  [[nodiscard]] auto is_log_dock_visible() const -> bool;

  void set_grid_visible(bool visible);
  [[nodiscard]] auto is_grid_visible() const -> bool;

  void set_highlight_active_layer(bool highlight);
  [[nodiscard]] auto highlight_active_layer() const -> bool;

  void set_embed_tilesets(bool embed);
  [[nodiscard]] auto embed_tilesets() const -> bool;

  void set_fold_tile_data(bool fold);
  [[nodiscard]] auto fold_tile_data() const -> bool;

  void set_indent_output(bool indent);
  [[nodiscard]] auto indent_output() const -> bool;

  void set_will_restore_layout(bool restore);
  [[nodiscard]] auto will_restore_layout() const -> bool;

  void set_will_restore_last_session(bool restore);
  [[nodiscard]] auto will_restore_last_session() const -> bool;

  void set_viewport_overlay_pos(OverlayPos pos);
  [[nodiscard]] auto viewport_overlay_pos() const -> OverlayPos;

  void set_viewport_overlay_show_fps(bool show);
  [[nodiscard]] auto viewport_overlay_show_fps() const -> bool;

  void set_command_capacity(usize capacity);
  [[nodiscard]] auto command_capacity() const -> usize;

  void set_preferred_format(std::string format);
  [[nodiscard]] auto preferred_format() const -> const std::string&;

  void set_preferred_tile_width(int32 width);
  [[nodiscard]] auto preferred_tile_width() const -> int32;

  void set_preferred_tile_height(int32 height);
  [[nodiscard]] auto preferred_tile_height() const -> int32;

  void set_font_size(int32 size);
  [[nodiscard]] auto font_size() const -> int32;

  void set_use_default_font(bool use);
  [[nodiscard]] auto use_default_font() const -> bool;

 private:
  struct Data;
  Unique<Data> mData;

  void set_flag(uint64 flag, bool value) noexcept;

  void reset_flag(uint64 flag) noexcept;

  [[nodiscard]] auto test_flag(uint64 flag) const noexcept -> bool;
};

}  // namespace tactile
