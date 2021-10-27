#include "preferences.hpp"

#include <filesystem>  // exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include <centurion.hpp>   // ...
#include <magic_enum.hpp>  // enum_name
#include <settings.pb.h>

#include "directories.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {
namespace {

inline const auto settings_path = GetPersistentFileDir() / "settings.bin";

constexpr CStr def_preferred_format = "YAML";
constexpr Theme def_theme = Theme::Nocturnal;
constexpr cen::color def_viewport_background{60, 60, 60};
constexpr usize def_command_capacity = 100;
constexpr int def_preferred_tile_width = 32;
constexpr int def_preferred_tile_height = 32;
constexpr int def_viewport_overlay_pos = cen::to_underlying(OverlayPos::BottomLeft);
constexpr bool def_show_grid = true;
constexpr bool def_embed_tilesets = false;
constexpr bool def_human_readable_output = true;
constexpr bool def_show_layer_dock = true;
constexpr bool def_show_tileset_dock = true;
constexpr bool def_show_properties_dock = true;
constexpr bool def_show_log_dock = false;
constexpr bool def_window_border = false;
constexpr bool def_restore_layout = true;
constexpr bool def_restore_last_session = true;

[[nodiscard]] auto MakeDefaultPreferences() -> Preferences
{
  return {.preferred_format = def_preferred_format,
          .theme = def_theme,
          .viewport_background = def_viewport_background,
          .command_capacity = def_command_capacity,
          .preferred_tile_width = def_preferred_tile_width,
          .preferred_tile_height = def_preferred_tile_height,
          .viewport_overlay_pos = def_viewport_overlay_pos,
          .embed_tilesets = def_embed_tilesets,
          .human_readable_output = def_human_readable_output,
          .show_grid = def_show_grid,
          .show_layer_dock = def_show_layer_dock,
          .show_tileset_dock = def_show_tileset_dock,
          .show_properties_dock = def_show_properties_dock,
          .show_log_dock = def_show_log_dock,
          .window_border = def_window_border,
          .restore_layout = def_restore_layout,
          .restore_last_session = def_restore_last_session};
}

inline Preferences settings = MakeDefaultPreferences();

}  // namespace

void LoadPreferences()
{
  settings = MakeDefaultPreferences();

  if (std::filesystem::exists(settings_path)) {
    std::ifstream stream{settings_path, std::ios::in | std::ios::binary};

    Proto::Settings cfg;
    if (cfg.ParseFromIstream(&stream)) {
      if (cfg.has_theme()) {
        settings.theme = static_cast<Theme>(cfg.theme());
      }

      if (cfg.has_viewport_background()) {
        const auto& color = cfg.viewport_background();
        settings.viewport_background.set_red(static_cast<uint8>(color.red()));
        settings.viewport_background.set_green(static_cast<uint8>(color.green()));
        settings.viewport_background.set_blue(static_cast<uint8>(color.blue()));
        settings.viewport_background.set_alpha(static_cast<uint8>(color.alpha()));
      }

      if (cfg.has_show_grid()) {
        settings.show_grid = cfg.show_grid();
      }

      if (cfg.has_window_border()) {
        settings.window_border = cfg.window_border();
      }

      if (cfg.has_command_capacity()) {
        settings.command_capacity = cfg.command_capacity();
      }

      if (cfg.has_restore_last_session()) {
        settings.restore_last_session = cfg.restore_last_session();
      }

      if (cfg.has_preferred_tile_width()) {
        settings.preferred_tile_width = cfg.preferred_tile_width();
      }

      if (cfg.has_preferred_tile_height()) {
        settings.preferred_tile_height = cfg.preferred_tile_height();
      }

      if (cfg.has_preferred_format()) {
        settings.preferred_format = cfg.preferred_format();
      }

      if (cfg.has_embed_tilesets()) {
        settings.embed_tilesets = cfg.embed_tilesets();
      }

      if (cfg.has_human_readable_output()) {
        settings.human_readable_output = cfg.human_readable_output();
      }

      if (cfg.has_show_tileset_dock()) {
        settings.show_tileset_dock = cfg.show_tileset_dock();
      }

      if (cfg.has_show_layer_dock()) {
        settings.show_layer_dock = cfg.show_layer_dock();
      }

      if (cfg.has_show_properties_dock()) {
        settings.show_properties_dock = cfg.show_properties_dock();
      }

      if (cfg.has_show_log_dock()) {
        settings.show_log_dock = cfg.show_log_dock();
      }

      if (cfg.has_restore_layout()) {
        settings.restore_layout = cfg.restore_layout();
      }

      if (cfg.has_viewport_overlay_pos()) {
        settings.viewport_overlay_pos = cfg.viewport_overlay_pos();
      }
    }
  }
  else {
    SavePreferences();
  }

  // clang-format off
  CENTURION_LOG_INFO("Theme: %s", magic_enum::enum_name(settings.theme).data());
  CENTURION_LOG_INFO("ViewportBackground: %s", settings.viewport_background.as_rgb().c_str());
  CENTURION_LOG_INFO("ShowGrid: %i", settings.show_grid);
  CENTURION_LOG_INFO("WindowBorder: %i", settings.window_border);

  CENTURION_LOG_INFO("CommandCapacity: %u", settings.command_capacity);
  CENTURION_LOG_INFO("RestoreLastSession: %i", settings.restore_last_session);
  CENTURION_LOG_INFO("PreferredTileWidth: %i", settings.preferred_tile_width);
  CENTURION_LOG_INFO("PreferredTileHeight: %i", settings.preferred_tile_height);

  CENTURION_LOG_INFO("PreferredFormat: %s", settings.preferred_format.c_str());
  CENTURION_LOG_INFO("EmbedTilesets: %i", settings.embed_tilesets);
  CENTURION_LOG_INFO("HumanReadableOutput: %i", settings.human_readable_output);

  CENTURION_LOG_INFO("ShowLayerDock: %i", settings.show_layer_dock);
  CENTURION_LOG_INFO("ShowLogDock: %i", settings.show_log_dock);
  CENTURION_LOG_INFO("ShowTilesetDock: %i", settings.show_tileset_dock);
  CENTURION_LOG_INFO("ShowPropertiesDock: %i", settings.show_properties_dock);
  CENTURION_LOG_INFO("ShowLogDock: %i", settings.show_log_dock);
  CENTURION_LOG_INFO("RestoreLayout: %i", settings.restore_layout);
  CENTURION_LOG_INFO("ViewportOverlayPos: %i", settings.viewport_overlay_pos);
  // clang-format on
}

void SavePreferences()
{
  Proto::Settings cfg;

  cfg.set_theme(static_cast<Proto::Theme>(settings.theme));
  cfg.set_show_grid(settings.show_grid);
  cfg.set_window_border(settings.window_border);

  {
    auto* background = cfg.mutable_viewport_background();
    background->set_red(settings.viewport_background.red());
    background->set_green(settings.viewport_background.green());
    background->set_blue(settings.viewport_background.blue());
    background->set_alpha(settings.viewport_background.alpha());
  }

  cfg.set_command_capacity(settings.command_capacity);
  cfg.set_restore_last_session(settings.restore_last_session);
  cfg.set_preferred_tile_width(settings.preferred_tile_width);
  cfg.set_preferred_tile_height(settings.preferred_tile_height);

  cfg.set_preferred_format(settings.preferred_format);
  cfg.set_embed_tilesets(settings.embed_tilesets);
  cfg.set_human_readable_output(settings.human_readable_output);

  cfg.set_show_tileset_dock(settings.show_tileset_dock);
  cfg.set_show_layer_dock(settings.show_layer_dock);
  cfg.set_show_properties_dock(settings.show_properties_dock);
  cfg.set_show_log_dock(settings.show_log_dock);
  cfg.set_restore_layout(settings.restore_layout);
  cfg.set_viewport_overlay_pos(Proto::OverlayPos{settings.viewport_overlay_pos});

  std::ofstream stream{settings_path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    CENTURION_LOG_WARN("Failed to save settings file!");
  }
}

void SetPreferences(Preferences prefs)
{
  settings = std::move(prefs);
}

auto GetPreferences() -> const Preferences&
{
  return settings;
}

namespace Prefs {

void ResetAppearancePreferences(Preferences& prefs)
{
  prefs.theme = def_theme;
  prefs.viewport_background = def_viewport_background;
  prefs.window_border = def_window_border;
  prefs.show_grid = def_show_grid;

  // Note, not technically an "appearance" setting, but it is to the user
  prefs.restore_layout = def_restore_layout;
}

void SetShowGrid(const bool show) noexcept
{
  settings.show_grid = show;
}

void ResetBehaviorPreferences(Preferences& prefs)
{
  prefs.command_capacity = def_command_capacity;
  prefs.restore_last_session = def_restore_last_session;
  prefs.preferred_tile_width = def_preferred_tile_width;
  prefs.preferred_tile_height = def_preferred_tile_height;
}

void ResetExportPreferences(Preferences& prefs)
{
  prefs.preferred_format = def_preferred_format;
  prefs.embed_tilesets = def_embed_tilesets;
  prefs.human_readable_output = def_human_readable_output;
}

void SetShowLayerDock(const bool visible) noexcept
{
  settings.show_layer_dock = visible;
}

void SetShowTilesetDock(const bool visible) noexcept
{
  settings.show_tileset_dock = visible;
}

void SetShowPropertiesDock(const bool visible) noexcept
{
  settings.show_properties_dock = visible;
}

void SetShowLogDock(const bool visible) noexcept
{
  settings.show_log_dock = visible;
}

void SetViewportOverlayPos(const OverlayPos pos) noexcept
{
  settings.viewport_overlay_pos = cen::to_underlying(pos);
}

auto GetPreferredFormat() -> const std::string&
{
  return settings.preferred_format;
}

auto GetShowGrid() noexcept -> bool
{
  return settings.show_grid;
}

auto GetEmbedTilesets() noexcept -> bool
{
  return settings.embed_tilesets;
}

auto GetHumanReadableOutput() noexcept -> bool
{
  return settings.human_readable_output;
}

auto GetShowLayerDock() noexcept -> bool
{
  return settings.show_layer_dock;
}

auto GetShowTilesetDock() noexcept -> bool
{
  return settings.show_tileset_dock;
}

auto GetShowPropertiesDock() noexcept -> bool
{
  return settings.show_properties_dock;
}

auto GetShowLogDock() noexcept -> bool
{
  return settings.show_log_dock;
}

auto GetTheme() noexcept -> Theme
{
  return settings.theme;
}

auto GetViewportBackground() noexcept -> const cen::color&
{
  return settings.viewport_background;
}

auto GetWindowBorder() noexcept -> bool
{
  return settings.window_border;
}

auto GetRestoreLayout() noexcept -> bool
{
  return settings.restore_layout;
}

auto GetViewportOverlayPos() noexcept -> OverlayPos
{
  return static_cast<OverlayPos>(settings.viewport_overlay_pos);
}

auto GetCommandCapacity() noexcept -> usize
{
  return settings.command_capacity;
}

auto GetPreferredTileWidth() noexcept -> int
{
  return settings.preferred_tile_width;
}

auto GetPreferredTileHeight() noexcept -> int
{
  return settings.preferred_tile_height;
}

auto GetRestoreLastSession() noexcept -> bool
{
  return settings.restore_last_session;
}

}  // namespace Prefs
}  // namespace Tactile
