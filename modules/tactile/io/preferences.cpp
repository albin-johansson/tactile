#include "preferences.hpp"

#include <filesystem>  // path, exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include <centurion.hpp>
#include <magic_enum.hpp>
#include <tactile_def.hpp>

#include "directories.hpp"
#include "logging.hpp"
#include <settings.pb.h>

#define PRINT_FLAG(Name, Mask) \
  LogInfo(Name "... {}", (prefs.flags & (Mask)) ? "yes" : "no")

namespace tactile {
namespace {

constexpr Theme gThemeDef = Theme::Nocturnal;
constexpr cen::color gViewportBackgroundDef{60, 60, 60};

constexpr CStr gPreferredFormatDef = "YAML";

constexpr usize gCommandCapacityDef = 100;

constexpr int gPreferredTileWidthDef = 32;
constexpr int gPreferredTileHeightDef = 32;
constexpr int gViewportOverlayPosDef = cen::to_underlying(OverlayPos::BottomLeft);

constexpr uint64 gFlagsDef =
    Preferences::show_grid | Preferences::indent_output | Preferences::show_layer_dock |
    Preferences::show_tileset_dock | Preferences::show_properties_dock |
    Preferences::show_component_dock | Preferences::restore_layout |
    Preferences::restore_last_session;

[[nodiscard]] auto get_file_path() -> const std::filesystem::path&
{
  static const auto path = persistent_file_dir() / "settings.bin";
  return path;
}

[[nodiscard]] auto MakeDefaultPreferences() -> Preferences
{
  return {.preferred_format = gPreferredFormatDef,
          .theme = gThemeDef,
          .viewport_background = gViewportBackgroundDef,
          .command_capacity = gCommandCapacityDef,
          .preferred_tile_width = gPreferredTileWidthDef,
          .preferred_tile_height = gPreferredTileHeightDef,
          .viewport_overlay_pos = gViewportOverlayPosDef,
          .flags = gFlagsDef};
}

inline Preferences settings = MakeDefaultPreferences();

void PrintPreferences(Preferences& prefs)
{
  LogInfo("Theme... {}", magic_enum::enum_name(prefs.theme));
  LogInfo("Viewport background... {}", prefs.viewport_background.as_rgb());

  LogInfo("Command capacity... {}", prefs.command_capacity);
  LogInfo("Preferred tile width... {}", prefs.preferred_tile_width);
  LogInfo("Preferred tile height... {}", prefs.preferred_tile_height);

  LogInfo("Preferred format... {}", prefs.preferred_format);
  LogInfo("Viewport overlay pos... {}", prefs.viewport_overlay_pos);

  PRINT_FLAG("Embed tilesets", Preferences::embed_tilesets);
  PRINT_FLAG("Indent output", Preferences::indent_output);
  PRINT_FLAG("Fold tile data", Preferences::fold_tile_data);

  PRINT_FLAG("Show grid", Preferences::show_grid);
  PRINT_FLAG("Show layer dock", Preferences::show_layer_dock);
  PRINT_FLAG("Show log dock", Preferences::show_log_dock);
  PRINT_FLAG("Show tileset dock", Preferences::show_tileset_dock);
  PRINT_FLAG("Show properties dock", Preferences::show_properties_dock);
  PRINT_FLAG("Show component dock", Preferences::show_component_dock);

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
  SetFlag(flag, gFlagsDef & flag);
}

void LoadPreferences()
{
  settings = MakeDefaultPreferences();

  const auto& path = get_file_path();
  if (std::filesystem::exists(path)) {
    std::ifstream stream{path, std::ios::in | std::ios::binary};

    proto::Settings cfg;
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

      if (cfg.has_show_component_dock()) {
        settings.SetFlag(Preferences::show_component_dock, cfg.show_component_dock());
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
    LogInfo("Did not find preferences file, assuming defaults");
    SavePreferences();
  }

  PrintPreferences(settings);
}

void SavePreferences()
{
  proto::Settings cfg;

  cfg.set_theme(static_cast<proto::Theme>(settings.theme));
  cfg.set_show_grid(prefs::GetShowGrid());
  cfg.set_window_border(prefs::GetWindowBorder());

  {
    auto* background = cfg.mutable_viewport_background();
    background->set_red(settings.viewport_background.red());
    background->set_green(settings.viewport_background.green());
    background->set_blue(settings.viewport_background.blue());
    background->set_alpha(settings.viewport_background.alpha());
  }

  cfg.set_command_capacity(settings.command_capacity);
  cfg.set_restore_last_session(prefs::GetRestoreLastSession());
  cfg.set_preferred_tile_width(settings.preferred_tile_width);
  cfg.set_preferred_tile_height(settings.preferred_tile_height);

  cfg.set_preferred_format(settings.preferred_format);
  cfg.set_embed_tilesets(prefs::GetEmbedTilesets());
  cfg.set_indent_output(prefs::GetIndentOutput());
  cfg.set_fold_tile_data(prefs::GetFoldTileData());

  cfg.set_show_tileset_dock(prefs::GetShowTilesetDock());
  cfg.set_show_layer_dock(prefs::GetShowLayerDock());
  cfg.set_show_properties_dock(prefs::GetShowPropertiesDock());
  cfg.set_show_log_dock(prefs::GetShowLogDock());
  cfg.set_show_component_dock(prefs::GetShowComponentDock());
  cfg.set_restore_layout(prefs::GetRestoreLayout());
  cfg.set_viewport_overlay_pos(proto::OverlayPos{settings.viewport_overlay_pos});

  std::ofstream stream{get_file_path(),
                       std::ios::out | std::ios::trunc | std::ios::binary};
  if (!cfg.SerializeToOstream(&stream)) {
    LogError("Failed to save preferences!");
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

namespace prefs {

void ResetAppearancePreferences(Preferences& prefs)
{
  prefs.theme = gThemeDef;
  prefs.viewport_background = gViewportBackgroundDef;

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
  prefs.command_capacity = gCommandCapacityDef;
  prefs.preferred_tile_width = gPreferredTileWidthDef;
  prefs.preferred_tile_height = gPreferredTileHeightDef;
  prefs.ResetFlag(Preferences::restore_last_session);
}

void ResetExportPreferences(Preferences& prefs)
{
  prefs.preferred_format = gPreferredFormatDef;
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

void SetShowComponentDock(const bool visible) noexcept
{
  settings.SetFlag(Preferences::show_component_dock, visible);
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

auto GetShowComponentDock() noexcept -> bool
{
  return settings.flags & Preferences::show_component_dock;
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

}  // namespace prefs
}  // namespace tactile
