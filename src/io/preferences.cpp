#include "preferences.hpp"

#include <centurion.hpp>  // CENTURION_LOG_INFO
#include <filesystem>     // exists
#include <init/ini.hpp>   // ini, read_ini, write_ini
#include <utility>        // move

#include "aliases/cstr.hpp"
#include "directories.hpp"
#include "gui/themes.hpp"

namespace Tactile {
namespace {

constexpr CStr file_name = "settings.ini";

constexpr CStr def_preferred_format = "JSON";
constexpr CStr def_theme = "Ash";
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
constexpr bool def_window_border = false;
constexpr bool def_restore_layout = true;
constexpr bool def_restore_last_session = true;

inline Preferences settings = {.preferred_format = def_preferred_format,
                               .theme = def_theme,
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
                               .window_border = def_window_border,
                               .restore_layout = def_restore_layout,
                               .restore_last_session = def_restore_last_session};

template <typename T>
void AddIfMissing(init::ini& ini, CStr section, CStr element, T value)
{
  auto& sec = ini[section];
  if (!sec.contains(element))
  {
    sec[element] = std::move(value);
  }
}

void WritePreferencesToFile(const Preferences& preferences = Preferences{})
{
  init::ini ini;

  ini["Appearance"]["Theme"] = preferences.theme;
  ini["Appearance"]["WindowBorder"] = preferences.window_border;
  ini["Appearance"]["ShowGrid"] = preferences.show_grid;

  ini["Behavior"]["CommandCapacity"] = preferences.command_capacity;
  ini["Behavior"]["RestoreLastSession"] = preferences.restore_last_session;
  ini["Behavior"]["PreferredTileWidth"] = preferences.preferred_tile_width;
  ini["Behavior"]["PreferredTileHeight"] = preferences.preferred_tile_height;

  ini["Export"]["PreferredFormat"] = preferences.preferred_format;
  ini["Export"]["EmbedTilesets"] = preferences.embed_tilesets;
  ini["Export"]["HumanReadableOutput"] = preferences.human_readable_output;

  ini["Widgets"]["ShowLayerDock"] = preferences.show_layer_dock;
  ini["Widgets"]["ShowTilesetDock"] = preferences.show_tileset_dock;
  ini["Widgets"]["ShowPropertiesDock"] = preferences.show_properties_dock;
  ini["Widgets"]["RestoreLayout"] = preferences.restore_layout;
  ini["Widgets"]["ViewportOverlayPos"] = preferences.viewport_overlay_pos;

  init::write_ini(ini, GetPersistentFileDir() / file_name);
}

// Read existing settings file and fill in any potentially missing settings
void ValidateExistingFile()
{
  const auto path = GetPersistentFileDir() / file_name;
  auto ini = init::read_ini(path);

  AddIfMissing(ini, "Appearance", "Theme", def_theme);
  AddIfMissing(ini, "Appearance", "ShowGrid", def_show_grid);
  AddIfMissing(ini, "Appearance", "WindowBorder", def_window_border);

  AddIfMissing(ini, "Behavior", "CommandCapacity", def_command_capacity);
  AddIfMissing(ini, "Behavior", "RestoreLastSession", def_restore_last_session);
  AddIfMissing(ini, "Behavior", "PreferredTileWidth", def_preferred_tile_width);
  AddIfMissing(ini, "Behavior", "PreferredTileHeight", def_preferred_tile_height);

  AddIfMissing(ini, "Export", "PreferredFormat", def_preferred_format);
  AddIfMissing(ini, "Export", "EmbedTilesets", def_embed_tilesets);
  AddIfMissing(ini, "Export", "HumanReadableOutput", def_human_readable_output);

  AddIfMissing(ini, "Widgets", "ShowLayerDock", def_show_layer_dock);
  AddIfMissing(ini, "Widgets", "ShowTilesetDock", def_show_tileset_dock);
  AddIfMissing(ini, "Widgets", "ShowPropertiesDock", def_show_properties_dock);
  AddIfMissing(ini, "Widgets", "RestoreLayout", def_restore_layout);
  AddIfMissing(ini, "Widgets", "ViewportOverlayPos", def_viewport_overlay_pos);

  init::write_ini(ini, path);
}

}  // namespace

void LoadPreferences()
{
  if (std::filesystem::exists(GetPersistentFileDir() / file_name))
  {
    ValidateExistingFile();
    const auto ini = init::read_ini(GetPersistentFileDir() / file_name);

    const auto& appearance = ini.at("Appearance");
    appearance.at("Theme").get_to(settings.theme);
    appearance.at("ShowGrid").get_to(settings.show_grid);
    appearance.at("WindowBorder").get_to(settings.window_border);

    const auto& behavior = ini.at("Behavior");
    behavior.at("CommandCapacity").get_to(settings.command_capacity);
    behavior.at("RestoreLastSession").get_to(settings.restore_last_session);
    behavior.at("PreferredTileWidth").get_to(settings.preferred_tile_width);
    behavior.at("PreferredTileHeight").get_to(settings.preferred_tile_height);

    const auto& exp = ini.at("Export");
    exp.at("PreferredFormat").get_to(settings.preferred_format);
    exp.at("EmbedTilesets").get_to(settings.embed_tilesets);
    exp.at("HumanReadableOutput").get_to(settings.human_readable_output);

    const auto& widgets = ini.at("Widgets");
    widgets.at("ShowTilesetDock").get_to(settings.show_tileset_dock);
    widgets.at("ShowLayerDock").get_to(settings.show_layer_dock);
    widgets.at("ShowPropertiesDock").get_to(settings.show_properties_dock);
    widgets.at("RestoreLayout").get_to(settings.restore_layout);
    widgets.at("ViewportOverlayPos").get_to(settings.viewport_overlay_pos);
  }
  else
  {
    WritePreferencesToFile();
  }

  // clang-format off
  CENTURION_LOG_INFO("Loaded preferences: \"%s\"", (GetPersistentFileDir() / file_name).string().c_str());
  CENTURION_LOG_INFO("  Appearance::Theme: %s", settings.theme.c_str());
  CENTURION_LOG_INFO("  Appearance::ShowGrid: %i", settings.show_grid);
  CENTURION_LOG_INFO("  Appearance::WindowBorder: %i", settings.window_border);
  CENTURION_LOG_INFO("  Behavior::CommandCapacity: %u", settings.command_capacity);
  CENTURION_LOG_INFO("  Behavior::RestoreLastSession: %i", settings.restore_last_session);
  CENTURION_LOG_INFO("  Behavior::PreferredTileWidth: %i", settings.preferred_tile_width);
  CENTURION_LOG_INFO("  Behavior::PreferredTileHeight: %i", settings.preferred_tile_height);
  CENTURION_LOG_INFO("  Export::PreferredFormat: %s", settings.preferred_format.c_str());
  CENTURION_LOG_INFO("  Export::EmbedTilesets: %i", settings.embed_tilesets);
  CENTURION_LOG_INFO("  Export::HumanReadableOutput: %i", settings.human_readable_output);
  CENTURION_LOG_INFO("  Widgets::ShowLayerDock: %i", settings.show_layer_dock);
  CENTURION_LOG_INFO("  Widgets::ShowTilesetDock: %i", settings.show_tileset_dock);
  CENTURION_LOG_INFO("  Widgets::ShowPropertiesDock: %i", settings.show_properties_dock);
  CENTURION_LOG_INFO("  Widgets::RestoreLayout: %i", settings.restore_layout);
  CENTURION_LOG_INFO("  Widgets::ViewportOverlayPos: %i", settings.viewport_overlay_pos);
  // clang-format on
}

void SavePreferences()
{
  WritePreferencesToFile(settings);
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
  prefs.window_border = def_window_border;
  prefs.show_grid = def_show_grid;

  // Note, not technically an "appearance" setting but it is to the user
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

auto GetTheme() noexcept -> const std::string&
{
  return settings.theme;
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
