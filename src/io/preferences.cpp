#include "preferences.hpp"

#include <filesystem>  // exists
#include <ini.hpp>     // ini_file, read_ini, write_ini
#include <string>      // string
#include <utility>     // move

#include "aliases/czstring.hpp"
#include "directories.hpp"
#include "gui/themes.hpp"

namespace Tactile {
namespace {

inline constexpr auto file_name = "settings.ini";

inline constexpr bool def_show_grid = true;
inline constexpr bool def_embed_tilesets = false;
inline constexpr bool def_human_readable_output = true;
inline constexpr bool def_show_layer_dock = true;
inline constexpr bool def_show_tileset_dock = true;
inline constexpr bool def_show_properties_dock = true;
inline constexpr bool def_window_border = false;
inline constexpr auto def_preferred_format = "JSON";
inline constexpr Theme def_theme = Theme::Ash;

struct Preferences final
{
  std::string preferred_format = def_preferred_format;
  Theme theme = def_theme;
  bool embed_tilesets = def_embed_tilesets;
  bool human_readable_output = def_human_readable_output;
  bool show_grid = def_show_grid;
  bool show_layer_dock = def_show_layer_dock;
  bool show_tileset_dock = def_show_tileset_dock;
  bool show_properties_dock = def_show_properties_dock;
  bool window_border = def_window_border;
};

inline Preferences settings;

template <typename T>
void AddIfMissing(rune::ini_file& ini, czstring section, czstring element, T value)
{
  auto& s = ini[section];
  if (!s.contains(element))
  {
    s[element] = std::move(value);
  }
}

void WritePreferencesToFile(const Preferences& preferences = Preferences{})
{
  rune::ini_file ini;

  ini["Appearance"]["Theme"] = GetThemeIndex(preferences.theme);
  ini["Appearance"]["WindowBorder"] = preferences.window_border;
  ini["Appearance"]["ShowGrid"] = preferences.show_grid;

  ini["Export"]["PreferredFormat"] = preferences.preferred_format;
  ini["Export"]["EmbedTilesets"] = preferences.embed_tilesets;
  ini["Export"]["HumanReadableOutput"] = preferences.human_readable_output;

  ini["Widgets"]["ShowLayerDock"] = preferences.show_layer_dock;
  ini["Widgets"]["ShowTilesetDock"] = preferences.show_tileset_dock;
  ini["Widgets"]["ShowPropertiesDock"] = preferences.show_properties_dock;

  rune::write_ini(ini, GetPersistentFileDir() / file_name);
}

// Read existing settings file and fill in any potentially missing settings
void ValidateExistingFile()
{
  const auto path = GetPersistentFileDir() / file_name;
  auto ini = rune::read_ini(path);

  AddIfMissing(ini, "Appearance", "Theme", GetThemeIndex(def_theme));
  AddIfMissing(ini, "Appearance", "ShowGrid", def_show_grid);
  AddIfMissing(ini, "Appearance", "WindowBorder", def_window_border);

  AddIfMissing(ini, "Export", "PreferredFormat", def_preferred_format);
  AddIfMissing(ini, "Export", "EmbedTilesets", def_embed_tilesets);
  AddIfMissing(ini, "Export", "HumanReadableOutput", def_human_readable_output);

  AddIfMissing(ini, "Widgets", "ShowLayerDock", def_show_layer_dock);
  AddIfMissing(ini, "Widgets", "ShowTilesetDock", def_show_tileset_dock);
  AddIfMissing(ini, "Widgets", "ShowPropertiesDock", def_show_properties_dock);

  rune::write_ini(ini, path);
}

}  // namespace

void LoadPreferences()
{
  if (std::filesystem::exists(GetPersistentFileDir() / file_name))
  {
    ValidateExistingFile();
    const auto ini = rune::read_ini(GetPersistentFileDir() / file_name);

    const auto& appearance = ini.at("Appearance");
    settings.theme = GetThemeFromIndex(appearance.at("Theme").get<int>());
    settings.show_grid = appearance.at("ShowGrid").get<bool>();
    settings.window_border = appearance.at("WindowBorder").get<bool>();

    const auto& exp = ini.at("Export");
    settings.preferred_format = exp.at("PreferredFormat").get<std::string>();
    settings.embed_tilesets = exp.at("EmbedTilesets").get<bool>();
    settings.human_readable_output = exp.at("HumanReadableOutput").get<bool>();

    const auto& widgets = ini.at("Widgets");
    settings.show_tileset_dock = widgets.at("ShowTilesetDock").get<bool>();
    settings.show_layer_dock = widgets.at("ShowLayerDock").get<bool>();
    settings.show_properties_dock = widgets.at("ShowPropertiesDock").get<bool>();
  }
  else
  {
    WritePreferencesToFile();
  }

  // clang-format off
  CENTURION_LOG_INFO("Loaded preferences: \"%s\"", (GetPersistentFileDir() / file_name).string().c_str());
  CENTURION_LOG_INFO("  Appearance::Theme: %i", settings.theme);
  CENTURION_LOG_INFO("  Appearance::ShowGrid: %i", settings.show_grid);
  CENTURION_LOG_INFO("  Appearance::WindowBorder: %i", settings.window_border);
  CENTURION_LOG_INFO("  Export::PreferredFormat: %s", settings.preferred_format.c_str());
  CENTURION_LOG_INFO("  Export::EmbedTilesets: %i", settings.embed_tilesets);
  CENTURION_LOG_INFO("  Export::HumanReadableOutput: %i", settings.human_readable_output);
  CENTURION_LOG_INFO("  Widgets::ShowLayerDock: %i", settings.show_layer_dock);
  CENTURION_LOG_INFO("  Widgets::ShowTilesetDock: %i", settings.show_tileset_dock);
  CENTURION_LOG_INFO("  Widgets::ShowPropertiesDock: %i", settings.show_properties_dock);
  // clang-format on
}

void SavePreferences()
{
  WritePreferencesToFile(settings);
}

namespace Prefs {

void SetPreferredFormat(std::string format)
{
  settings.preferred_format = std::move(format);
}

void SetShowGrid(const bool show) noexcept
{
  settings.show_grid = show;
}

void SetEmbedTilesets(const bool embed) noexcept
{
  settings.embed_tilesets = embed;
}

void SetHumanReadableOutput(const bool readable) noexcept
{
  settings.human_readable_output = readable;
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

void SetTheme(const Theme theme) noexcept
{
  settings.theme = theme;
}

void SetWindowBorder(const bool enabled) noexcept
{
  settings.window_border = enabled;
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

auto GetTheme() noexcept -> Theme
{
  return settings.theme;
}

auto GetWindowBorder() noexcept -> bool
{
  return settings.window_border;
}

}  // namespace Prefs
}  // namespace Tactile
