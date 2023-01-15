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

#include "settings.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "common/type/path.hpp"
#include "io/directories.hpp"
#include "io/file.hpp"
#include "io/proto/proto.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto get_settings_file_path() -> const Path&
{
  static const auto path = persistent_file_dir() / "settings.bin";
  return path;
}

[[nodiscard]] auto from_proto(const proto::Color& color) -> Color
{
  const auto r = static_cast<uint8>(color.red());
  const auto g = static_cast<uint8>(color.green());
  const auto b = static_cast<uint8>(color.blue());
  const auto a = static_cast<uint8>(color.alpha());
  return Color {r, g, b, a};
}

void to_proto(const Color& color, proto::Color* out)
{
  out->set_red(color.red);
  out->set_green(color.green);
  out->set_blue(color.blue);
  out->set_alpha(color.alpha);
}

[[nodiscard]] auto parse_settings_from_file(const Path& path) -> Maybe<Settings>
{
  auto stream = read_file(path, FileType::Binary);

  proto::Settings cfg;
  if (cfg.ParseFromIstream(&stream)) {
    Settings settings;

    if (cfg.has_language()) {
      settings.set_language(static_cast<Lang>(cfg.language()));
    }

    if (cfg.has_theme()) {
      settings.set_theme(static_cast<ui::EditorTheme>(cfg.theme()));
    }

    if (cfg.has_viewport_background()) {
      settings.set_viewport_bg_color(from_proto(cfg.viewport_background()));
    }

    if (cfg.has_grid_color()) {
      settings.set_grid_color(from_proto(cfg.grid_color()));
    }

    if (cfg.has_show_grid()) {
      settings.set_flag(SETTINGS_SHOW_GRID_BIT, cfg.show_grid());
    }

    if (cfg.has_highlight_active_layer()) {
      settings.set_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT,
                        cfg.highlight_active_layer());
    }

    if (cfg.has_window_border()) {
      settings.set_flag(SETTINGS_WINDOW_BORDER_BIT, cfg.window_border());
    }

    if (cfg.has_command_capacity()) {
      settings.set_command_capacity(cfg.command_capacity());
    }

    if (cfg.has_restore_last_session()) {
      settings.set_flag(SETTINGS_RESTORE_LAST_SESSION_BIT, cfg.restore_last_session());
    }

    if (cfg.has_preferred_tile_width()) {
      auto tile_size = settings.get_preferred_tile_size();
      tile_size.x = cfg.preferred_tile_width();
      settings.set_preferred_tile_size(tile_size);
    }

    if (cfg.has_preferred_tile_height()) {
      auto tile_size = settings.get_preferred_tile_size();
      tile_size.y = cfg.preferred_tile_height();
      settings.set_preferred_tile_size(tile_size);
    }

    if (cfg.has_preferred_format()) {
      settings.set_preferred_format(cfg.preferred_format());
    }

    if (cfg.has_embed_tilesets()) {
      settings.set_flag(SETTINGS_EMBED_TILESETS_BIT, cfg.embed_tilesets());
    }

    if (cfg.has_indent_output()) {
      settings.set_flag(SETTINGS_INDENT_OUTPUT_BIT, cfg.indent_output());
    }

    if (cfg.has_fold_tile_data()) {
      settings.set_flag(SETTINGS_FOLD_TILE_DATA_BIT, cfg.fold_tile_data());
    }

    if (cfg.has_show_tileset_dock()) {
      settings.set_flag(SETTINGS_SHOW_TILESET_DOCK_BIT, cfg.show_tileset_dock());
    }

    if (cfg.has_show_layer_dock()) {
      settings.set_flag(SETTINGS_SHOW_LAYER_DOCK_BIT, cfg.show_layer_dock());
    }

    if (cfg.has_show_property_dock()) {
      settings.set_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT, cfg.show_property_dock());
    }

    if (cfg.has_show_log_dock()) {
      settings.set_flag(SETTINGS_SHOW_LOG_DOCK_BIT, cfg.show_log_dock());
    }

    if (cfg.has_show_animation_dock()) {
      settings.set_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT, cfg.show_animation_dock());
    }

    if (cfg.has_show_component_dock()) {
      settings.set_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT, cfg.show_component_dock());
    }

    if (cfg.has_restore_layout()) {
      settings.set_flag(SETTINGS_RESTORE_LAYOUT_BIT, cfg.restore_layout());
    }

    if (cfg.has_viewport_overlay_pos()) {
      settings.set_viewport_overlay_pos(
          static_cast<OverlayPos>(cfg.viewport_overlay_pos()));
    }

    if (cfg.has_viewport_overlay_show_fps()) {
      settings.set_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT,
                        cfg.viewport_overlay_show_fps());
    }

    if (cfg.has_use_default_font()) {
      settings.set_flag(SETTINGS_USE_DEFAULT_FONT_BIT, cfg.use_default_font());
    }

    if (cfg.has_font_size()) {
      settings.set_font_size(cfg.font_size());
    }

    return std::move(settings);
  }

  return nothing;
}

}  // namespace

auto load_settings_from_disk() -> Settings
{
  const auto& settings_path = get_settings_file_path();
  if (fs::exists(settings_path)) {
    if (auto settings = parse_settings_from_file(settings_path)) {
      return std::move(*settings);
    }
    else {
      spdlog::warn("Could not read settings file, assuming defaults");
      return Settings {};
    }
  }
  else {
    spdlog::warn("Could not find settings file, assuming defaults");
    return Settings {};
  }
}

void save_settings_to_disk(const Settings& settings)
{
  spdlog::debug("Saving settings...");
  proto::Settings cfg;

  cfg.set_language(static_cast<proto::Lang>(settings.get_language()));
  cfg.set_theme(static_cast<proto::Theme>(settings.get_theme()));
  cfg.set_show_grid(settings.test_flag(SETTINGS_SHOW_GRID_BIT));
  cfg.set_highlight_active_layer(settings.test_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT));
  cfg.set_window_border(settings.test_flag(SETTINGS_WINDOW_BORDER_BIT));

  to_proto(settings.get_viewport_bg_color(), cfg.mutable_viewport_background());
  to_proto(settings.get_grid_color(), cfg.mutable_grid_color());

  cfg.set_command_capacity(settings.get_command_capacity());
  cfg.set_restore_last_session(settings.test_flag(SETTINGS_RESTORE_LAST_SESSION_BIT));
  cfg.set_preferred_tile_width(settings.get_preferred_tile_size().x);
  cfg.set_preferred_tile_height(settings.get_preferred_tile_size().y);

  cfg.set_preferred_format(settings.get_preferred_format());
  cfg.set_embed_tilesets(settings.test_flag(SETTINGS_EMBED_TILESETS_BIT));
  cfg.set_indent_output(settings.test_flag(SETTINGS_INDENT_OUTPUT_BIT));
  cfg.set_fold_tile_data(settings.test_flag(SETTINGS_FOLD_TILE_DATA_BIT));

  cfg.set_show_tileset_dock(settings.test_flag(SETTINGS_SHOW_TILESET_DOCK_BIT));
  cfg.set_show_layer_dock(settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT));
  cfg.set_show_property_dock(settings.test_flag(SETTINGS_SHOW_PROPERTY_DOCK_BIT));
  cfg.set_show_component_dock(settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT));
  cfg.set_show_log_dock(settings.test_flag(SETTINGS_SHOW_LOG_DOCK_BIT));
  cfg.set_show_animation_dock(settings.test_flag(SETTINGS_SHOW_ANIMATION_DOCK_BIT));
  cfg.set_restore_layout(settings.test_flag(SETTINGS_RESTORE_LAYOUT_BIT));
  cfg.set_viewport_overlay_show_fps(
      settings.test_flag(SETTINGS_SHOW_VIEWPORT_OVERLAY_FPS_BIT));
  cfg.set_viewport_overlay_pos(
      static_cast<proto::OverlayPos>(settings.get_viewport_overlay_pos()));

  cfg.set_use_default_font(settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT));
  cfg.set_font_size(settings.get_font_size());

  const auto& path = get_settings_file_path();
  auto stream = write_file(path, FileType::Binary);

  if (!cfg.SerializeToOstream(&stream)) {
    spdlog::error("Failed to save settings!");
  }
}

}  // namespace tactile::io
