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

#include "preferences.hpp"

#include <fstream>  // ifstream, ofstream
#include <ios>      // ios
#include <utility>  // move

#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "core/common/filesystem.hpp"
#include "io/directories.hpp"
#include "io/persistence/proto.hpp"

namespace tactile::io {
namespace {

inline PreferenceState _settings;

[[nodiscard]] auto _get_preference_file_path() -> const fs::path&
{
  static const auto path = persistent_file_dir() / "settings.bin";
  return path;
}

[[nodiscard]] auto _parse_preferences(const fs::path& path) -> PreferenceState
{
  PreferenceState result {};
  std::ifstream   stream {path, std::ios::in | std::ios::binary};

  proto::Settings cfg;
  if (cfg.ParseFromIstream(&stream)) {
    if (cfg.has_theme()) {
      result.theme = static_cast<ui::EditorTheme>(cfg.theme());
    }

    if (cfg.has_viewport_background()) {
      const auto& color = cfg.viewport_background();
      result.viewport_background.set_red(static_cast<uint8>(color.red()));
      result.viewport_background.set_green(static_cast<uint8>(color.green()));
      result.viewport_background.set_blue(static_cast<uint8>(color.blue()));
      result.viewport_background.set_alpha(static_cast<uint8>(color.alpha()));
    }

    if (cfg.has_show_grid()) {
      result.show_grid = cfg.show_grid();
    }

    if (cfg.has_highlight_active_layer()) {
      result.highlight_active_layer = cfg.highlight_active_layer();
    }

    if (cfg.has_window_border()) {
      result.window_border = cfg.window_border();
    }

    if (cfg.has_command_capacity()) {
      result.command_capacity = cfg.command_capacity();
    }

    if (cfg.has_restore_last_session()) {
      result.restore_last_session = cfg.restore_last_session();
    }

    if (cfg.has_preferred_tile_width()) {
      result.preferred_tile_size.x = cfg.preferred_tile_width();
    }

    if (cfg.has_preferred_tile_height()) {
      result.preferred_tile_size.y = cfg.preferred_tile_height();
    }

    if (cfg.has_preferred_format()) {
      result.preferred_format = cfg.preferred_format();
    }

    if (cfg.has_embed_tilesets()) {
      result.embed_tilesets = cfg.embed_tilesets();
    }

    if (cfg.has_indent_output()) {
      result.indent_output = cfg.indent_output();
    }

    if (cfg.has_fold_tile_data()) {
      result.fold_tile_data = cfg.fold_tile_data();
    }

    if (cfg.has_show_tileset_dock()) {
      result.show_tileset_dock = cfg.show_tileset_dock();
    }

    if (cfg.has_show_layer_dock()) {
      result.show_layer_dock = cfg.show_layer_dock();
    }

    if (cfg.has_show_property_dock()) {
      result.show_property_dock = cfg.show_property_dock();
    }

    if (cfg.has_show_log_dock()) {
      result.show_log_dock = cfg.show_log_dock();
    }

    if (cfg.has_show_component_dock()) {
      result.show_component_dock = cfg.show_component_dock();
    }

    if (cfg.has_restore_layout()) {
      result.restore_layout = cfg.restore_layout();
    }

    if (cfg.has_viewport_overlay_pos()) {
      result.viewport_overlay_pos = static_cast<OverlayPos>(cfg.viewport_overlay_pos());
    }

    if (cfg.has_viewport_overlay_show_fps()) {
      result.show_viewport_overlay_fps = cfg.viewport_overlay_show_fps();
    }

    if (cfg.has_use_default_font()) {
      result.use_default_font = cfg.use_default_font();
    }

    if (cfg.has_font_size()) {
      result.font_size = cfg.font_size();
    }
  }

  return result;
}

}  // namespace

void load_preferences()
{
  _settings = PreferenceState {};

  const auto& path = _get_preference_file_path();
  if (fs::exists(path)) {
    _settings = _parse_preferences(path);
  }
  else {
    spdlog::info("Did not find a preferences file, assuming defaults");
    save_preferences();
  }

  print_preferences();
}

void save_preferences()
{
  proto::Settings cfg;

  cfg.set_theme(static_cast<proto::Theme>(_settings.theme));
  cfg.set_show_grid(_settings.show_grid);
  cfg.set_highlight_active_layer(_settings.highlight_active_layer);
  cfg.set_window_border(_settings.window_border);

  {
    auto* background = cfg.mutable_viewport_background();
    background->set_red(_settings.viewport_background.red());
    background->set_green(_settings.viewport_background.green());
    background->set_blue(_settings.viewport_background.blue());
    background->set_alpha(_settings.viewport_background.alpha());
  }

  cfg.set_command_capacity(_settings.command_capacity);
  cfg.set_restore_last_session(_settings.restore_last_session);
  cfg.set_preferred_tile_width(_settings.preferred_tile_size.x);
  cfg.set_preferred_tile_height(_settings.preferred_tile_size.y);

  cfg.set_preferred_format(_settings.preferred_format);
  cfg.set_embed_tilesets(_settings.embed_tilesets);
  cfg.set_indent_output(_settings.indent_output);
  cfg.set_fold_tile_data(_settings.fold_tile_data);

  cfg.set_show_tileset_dock(_settings.show_tileset_dock);
  cfg.set_show_layer_dock(_settings.show_layer_dock);
  cfg.set_show_property_dock(_settings.show_property_dock);
  cfg.set_show_log_dock(_settings.show_log_dock);
  cfg.set_show_component_dock(_settings.show_component_dock);
  cfg.set_restore_layout(_settings.restore_layout);
  cfg.set_viewport_overlay_pos(
      static_cast<proto::OverlayPos>(_settings.viewport_overlay_pos));
  cfg.set_viewport_overlay_show_fps(_settings.show_viewport_overlay_fps);

  cfg.set_use_default_font(_settings.use_default_font);
  cfg.set_font_size(_settings.font_size);

  const auto&   path = _get_preference_file_path();
  std::ofstream stream {path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    spdlog::error("Failed to save preferences!");
  }
}

void print_preferences()
{
  spdlog::debug("Theme... {}", magic_enum::enum_name(_settings.theme));
  spdlog::debug("Viewport background... {}", _settings.viewport_background.as_rgb());

  spdlog::debug("Command capacity... {}", _settings.command_capacity);
  spdlog::debug("Preferred tile width... {}", _settings.preferred_tile_size.x);
  spdlog::debug("Preferred tile height... {}", _settings.preferred_tile_size.y);

  spdlog::debug("Preferred format... {}", _settings.preferred_format);
  spdlog::debug("Viewport overlay pos... {}",
                magic_enum::enum_name(_settings.viewport_overlay_pos));
  spdlog::debug("Show FPS in viewport overlay... {}",
                _settings.show_viewport_overlay_fps);

  spdlog::debug("Font size... {}", _settings.font_size);
  spdlog::debug("Use default font... {}", _settings.use_default_font);

  spdlog::debug("Embed tilesets... {}", _settings.embed_tilesets);
  spdlog::debug("Indent output... {}", _settings.indent_output);
  spdlog::debug("Fold tile data... {}", _settings.fold_tile_data);

  spdlog::debug("Show grid... {}", _settings.show_grid);
  spdlog::debug("Highlight active layer... {}", _settings.highlight_active_layer);
  spdlog::debug("Show layer dock... {}", _settings.show_layer_dock);
  spdlog::debug("Show log dock... {}", _settings.show_log_dock);
  spdlog::debug("Show tileset dock... {}", _settings.show_tileset_dock);
  spdlog::debug("Show property dock... {}", _settings.show_property_dock);
  spdlog::debug("Show component dock... {}", _settings.show_component_dock);

  spdlog::debug("Window border... {}", _settings.window_border);
  spdlog::debug("Restore layout... {}", _settings.restore_layout);
  spdlog::debug("Restore last session... {}", _settings.restore_last_session);
}

void set_preferences(PreferenceState prefs)
{
  _settings = std::move(prefs);
}

auto get_preferences() -> PreferenceState&
{
  return _settings;
}

}  // namespace tactile::io
