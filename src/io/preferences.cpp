#include "preferences.hpp"

#include <filesystem>  // exists
#include <ini.hpp>     // ini_file, read_ini, write_ini
#include <string>      // string
#include <utility>     // move

#include "directories.hpp"

namespace tactile {
namespace {

inline constexpr auto file_name = "settings.ini";

inline constexpr bool def_show_grid = true;
inline constexpr bool def_embed_tilesets = false;
inline constexpr bool def_human_readable_output = true;
inline constexpr auto def_preferred_format = "JSON";

struct Preferences final
{
  std::string preferred_format = def_preferred_format;
  bool embed_tilesets = def_embed_tilesets;
  bool human_readable_output = def_human_readable_output;
  bool show_grid = def_show_grid;
};

inline Preferences settings;

void WritePreferencesToFile(const Preferences& preferences = Preferences{})
{
  rune::ini_file ini;

  ini["Graphics"]["ShowGrid"] = preferences.show_grid;

  ini["Export"]["PreferredFormat"] = preferences.preferred_format;
  ini["Export"]["EmbedTilesets"] = preferences.embed_tilesets;
  ini["Export"]["HumanReadableOutput"] = preferences.human_readable_output;

  rune::write_ini(ini, GetPersistentFileDir() / file_name);
}

// Read existing settings file and fill in any potentially missing settings
void ValidateExistingFile()
{
  auto ini = rune::read_ini(GetPersistentFileDir() / file_name);

  auto& gfx = ini["Graphics"];
  auto& exp = ini["Export"];

  if (!gfx.contains("ShowGrid"))
  {
    gfx["ShowGrid"] = def_show_grid;
  }

  if (!exp.contains("PreferredFormat"))
  {
    exp["PreferredFormat"] = def_preferred_format;
  }

  if (!exp.contains("EmbedTilesets"))
  {
    exp["EmbedTilesets"] = def_embed_tilesets;
  }

  if (!exp.contains("HumanReadableOutput"))
  {
    exp["HumanReadableOutput"] = def_human_readable_output;
  }

  rune::write_ini(ini, GetPersistentFileDir() / file_name);
}

}  // namespace

void LoadPreferences()
{
  if (std::filesystem::exists(GetPersistentFileDir() / file_name))
  {
    ValidateExistingFile();
    const auto ini = rune::read_ini(GetPersistentFileDir() / file_name);

    const auto& gfx = ini.at("Graphics");
    settings.show_grid = gfx.at("ShowGrid").get<bool>();

    const auto& exp = ini.at("Export");
    settings.preferred_format = exp.at("PreferredFormat").get<std::string>();
    settings.embed_tilesets = exp.at("EmbedTilesets").get<bool>();
    settings.human_readable_output = exp.at("HumanReadableOutput").get<bool>();
  }
  else
  {
    WritePreferencesToFile();
  }

  CENTURION_LOG_INFO("Loaded preferences: \"%s\"",
                     (GetPersistentFileDir() / file_name).string().c_str());
  CENTURION_LOG_INFO("  Graphics::ShowGrid: %i", settings.show_grid);
  CENTURION_LOG_INFO("  Export::PreferredFormat: %s",
                     settings.preferred_format.c_str());
  CENTURION_LOG_INFO("  Export::EmbedTilesets: %i", settings.embed_tilesets);
  CENTURION_LOG_INFO("  Export::HumanReadableOutput: %i",
                     settings.human_readable_output);
}

void SavePreferences()
{
  WritePreferencesToFile(settings);
}

namespace prefs {

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

}  // namespace prefs
}  // namespace tactile
