#include "preferences.hpp"

#include <filesystem>  // exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include <tactile_def.hpp>

#include <centurion.hpp>   // ...
#include <magic_enum.hpp>  // enum_name

#include "directories.hpp"

#include <settings.pb.h>

#define PRINT_FLAG(Name, Mask) \
  CENTURION_LOG_INFO(Name "... %s", (prefs.flags & (Mask)) ? "yes" : "no")

namespace Tactile {
namespace {

inline const auto settings_path = GetPersistentFileDir() / "settings.bin";

constexpr Theme def_theme = Theme::Nocturnal;

constexpr CStr def_preferred_format = "YAML";

constexpr usize def_command_capacity = 100;

constexpr int def_preferred_tile_width = 32;
constexpr int def_preferred_tile_height = 32;

constexpr cen::color def_viewport_background{60, 60, 60};
constexpr int def_viewport_overlay_pos = cen::to_underlying(OverlayPos::BottomLeft);

constexpr uint64 def_flags =
    Preferences::show_grid | Preferences::indent_output | Preferences::show_layer_dock |
    Preferences::show_tileset_dock | Preferences::show_properties_dock |
    Preferences::restore_layout | Preferences::restore_last_session;

[[nodiscard]] auto MakeDefaultPreferences() -> Preferences
{
  return {.preferred_format = def_preferred_format,
          .theme = def_theme,
          .viewport_background = def_viewport_background,
          .command_capacity = def_command_capacity,
          .preferred_tile_width = def_preferred_tile_width,
          .preferred_tile_height = def_preferred_tile_height,
          .viewport_overlay_pos = def_viewport_overlay_pos,
          .flags = def_flags};
}

inline Preferences settings = MakeDefaultPreferences();

void PrintPreferences(Preferences& prefs)
{
  CENTURION_LOG_INFO("Theme... %s", magic_enum::enum_name(prefs.theme).data());
  CENTURION_LOG_INFO("Viewport background... %s",
                     prefs.viewport_background.as_rgb().c_str());

  CENTURION_LOG_INFO("Command capacity... %u", prefs.command_capacity);
  CENTURION_LOG_INFO("Preferred tile width... %i", prefs.preferred_tile_width);
  CENTURION_LOG_INFO("Preferred tile height... %i", prefs.preferred_tile_height);

  CENTURION_LOG_INFO("Preferred format... %s", prefs.preferred_format.c_str());
  CENTURION_LOG_INFO("Viewport overlay pos... %i", prefs.viewport_overlay_pos);

  PRINT_FLAG("Embed tilesets", Preferences::embed_tilesets);
  PRINT_FLAG("Indent output", Preferences::indent_output);
  PRINT_FLAG("Fold tile data", Preferences::fold_tile_data);

  PRINT_FLAG("Show grid", Preferences::show_grid);
  PRINT_FLAG("Show layer dock", Preferences::show_layer_dock);
  PRINT_FLAG("Show log dock", Preferences::show_log_dock);
  PRINT_FLAG("Show tileset dock", Preferences::show_tileset_dock);
  PRINT_FLAG("Show properties dock", Preferences::show_properties_dock);

  PRINT_FLAG("Window border", Preferences::window_border);
  PRINT_FLAG("Restore layout", Preferences::restore_layout);
  PRINT_FLAG("Restore last session", Preferences::restore_last_session);
}

}  // namespace

void Preferences::SetFlag(const uint64 flag, const bool value) noexcept
{
  if (value) {
    flags |= flag;
  }
  else {
    flags &= ~flag;
  }
}

void Preferences::ResetFlag(const uint64 flag) noexcept
{
  SetFlag(flag, def_flags & flag);
}

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
        settings.SetFlag(Preferences::show_grid, cfg.show_grid());
      }

      if (cfg.has_window_border()) {
        settings.SetFlag(Preferences::window_border, cfg.window_border());
      }

      if (cfg.has_command_capacity()) {
        settings.command_capacity = cfg.command_capacity();
      }

      if (cfg.has_restore_last_session()) {
        settings.SetFlag(Preferences::restore_last_session, cfg.restore_last_session());
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
        settings.SetFlag(Preferences::embed_tilesets, cfg.embed_tilesets());
      }

      if (cfg.has_indent_output()) {
        settings.SetFlag(Preferences::indent_output, cfg.indent_output());
      }

      if (cfg.has_fold_tile_data()) {
        settings.SetFlag(Preferences::fold_tile_data, cfg.fold_tile_data());
      }

      if (cfg.has_show_tileset_dock()) {
        settings.SetFlag(Preferences::show_tileset_dock, cfg.show_tileset_dock());
      }

      if (cfg.has_show_layer_dock()) {
        settings.SetFlag(Preferences::show_layer_dock, cfg.show_layer_dock());
      }

      if (cfg.has_show_properties_dock()) {
        settings.SetFlag(Preferences::show_properties_dock, cfg.show_properties_dock());
      }

      if (cfg.has_show_log_dock()) {
        settings.SetFlag(Preferences::show_log_dock, cfg.show_log_dock());
      }

      if (cfg.has_restore_layout()) {
        settings.SetFlag(Preferences::restore_layout, cfg.restore_layout());
      }

      if (cfg.has_viewport_overlay_pos()) {
        settings.viewport_overlay_pos = cfg.viewport_overlay_pos();
      }
    }
  }
  else {
    cen::log::info("Settings file not found, using default preferences");
    SavePreferences();
  }

  PrintPreferences(settings);
}

void SavePreferences()
{
  Proto::Settings cfg;

  cfg.set_theme(static_cast<Proto::Theme>(settings.theme));
  cfg.set_show_grid(Prefs::GetShowGrid());
  cfg.set_window_border(Prefs::GetWindowBorder());

  {
    auto* background = cfg.mutable_viewport_background();
    background->set_red(settings.viewport_background.red());
    background->set_green(settings.viewport_background.green());
    background->set_blue(settings.viewport_background.blue());
    background->set_alpha(settings.viewport_background.alpha());
  }

  cfg.set_command_capacity(settings.command_capacity);
  cfg.set_restore_last_session(Prefs::GetRestoreLastSession());
  cfg.set_preferred_tile_width(settings.preferred_tile_width);
  cfg.set_preferred_tile_height(settings.preferred_tile_height);

  cfg.set_preferred_format(settings.preferred_format);
  cfg.set_embed_tilesets(Prefs::GetEmbedTilesets());
  cfg.set_indent_output(Prefs::GetIndentOutput());
  cfg.set_fold_tile_data(Prefs::GetFoldTileData());

  cfg.set_show_tileset_dock(Prefs::GetShowTilesetDock());
  cfg.set_show_layer_dock(Prefs::GetShowLayerDock());
  cfg.set_show_properties_dock(Prefs::GetShowPropertiesDock());
  cfg.set_show_log_dock(Prefs::GetShowLogDock());
  cfg.set_restore_layout(Prefs::GetRestoreLayout());
  cfg.set_viewport_overlay_pos(Proto::OverlayPos{settings.viewport_overlay_pos});

  std::ofstream stream{settings_path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    cen::log::error("Failed to save settings!");
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

  prefs.ResetFlag(Preferences::window_border);
  prefs.ResetFlag(Preferences::show_grid);

  // Note, not technically an "appearance" setting, but it is to the user
  prefs.ResetFlag(Preferences::restore_layout);
}

void SetShowGrid(const bool show) noexcept
{
  settings.SetFlag(Preferences::show_grid, show);
}

void ResetBehaviorPreferences(Preferences& prefs)
{
  prefs.command_capacity = def_command_capacity;
  prefs.preferred_tile_width = def_preferred_tile_width;
  prefs.preferred_tile_height = def_preferred_tile_height;
  prefs.ResetFlag(Preferences::restore_last_session);
}

void ResetExportPreferences(Preferences& prefs)
{
  prefs.preferred_format = def_preferred_format;
  prefs.ResetFlag(Preferences::embed_tilesets);
  prefs.ResetFlag(Preferences::indent_output);
  prefs.ResetFlag(Preferences::fold_tile_data);
}

void ResetDockVisibilities() noexcept
{
  settings.ResetFlag(Preferences::show_layer_dock);
  settings.ResetFlag(Preferences::show_tileset_dock);
  settings.ResetFlag(Preferences::show_properties_dock);
  settings.ResetFlag(Preferences::show_log_dock);
}

void SetShowLayerDock(const bool visible) noexcept
{
  settings.SetFlag(Preferences::show_layer_dock, visible);
}

void SetShowTilesetDock(const bool visible) noexcept
{
  settings.SetFlag(Preferences::show_tileset_dock, visible);
}

void SetShowPropertiesDock(const bool visible) noexcept
{
  settings.SetFlag(Preferences::show_properties_dock, visible);
}

void SetShowLogDock(const bool visible) noexcept
{
  settings.SetFlag(Preferences::show_log_dock, visible);
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
  return settings.flags & Preferences::show_grid;
}

auto GetEmbedTilesets() noexcept -> bool
{
  return settings.flags & Preferences::embed_tilesets;
}

auto GetFoldTileData() noexcept -> bool
{
  return settings.flags & Preferences::fold_tile_data;
}

auto GetIndentOutput() noexcept -> bool
{
  return settings.flags & Preferences::indent_output;
}

auto GetShowLayerDock() noexcept -> bool
{
  return settings.flags & Preferences::show_layer_dock;
}

auto GetShowTilesetDock() noexcept -> bool
{
  return settings.flags & Preferences::show_tileset_dock;
}

auto GetShowPropertiesDock() noexcept -> bool
{
  return settings.flags & Preferences::show_properties_dock;
}

auto GetShowLogDock() noexcept -> bool
{
  return settings.flags & Preferences::show_log_dock;
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
  return settings.flags & Preferences::window_border;
}

auto GetRestoreLayout() noexcept -> bool
{
  return settings.flags & Preferences::restore_layout;
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
  return settings.flags & Preferences::restore_last_session;
}

}  // namespace Prefs
}  // namespace Tactile
