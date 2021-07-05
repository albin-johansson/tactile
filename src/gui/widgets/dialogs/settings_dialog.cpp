#include "settings_dialog.hpp"

#include <imgui.h>

#include "gui/themes.hpp"
#include "gui/widgets/common/help_marker.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool restore_last_session = false;
inline bool rmb_stamp_as_eraser = false;
inline bool generate_default_values = false;

void ShowGeneralTab()
{
  if (ImGui::BeginTabItem("General"))
  {
    ImGui::Spacing();

    ImGui::Text("Startup");
    ImGui::Checkbox("Restore last session", &restore_last_session);

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Behavior");
    ImGui::Checkbox("RMB with stamp tool works as eraser",
                    &rmb_stamp_as_eraser);

    ImGui::EndTabItem();
  }
}

void ShowThemeBar()
{
  if (ImGui::BeginTabItem("Theme"))
  {
    ImGui::Spacing();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Theme:");

    ImGui::SameLine();
    auto themeIndex = GetThemeIndex(Prefs::GetTheme());
    if (ImGui::Combo("##ThemeCombo", &themeIndex, theme_options))
    {
      const auto theme = GetThemeFromIndex(themeIndex);
      Prefs::SetTheme(theme);
      ApplyTheme(ImGui::GetStyle(), theme);
    }

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export"))
  {
    ImGui::Spacing();

    bool embedTilesets = Prefs::GetEmbedTilesets();
    if (ImGui::Checkbox("Embed tilesets", &embedTilesets))
    {
      Prefs::SetEmbedTilesets(embedTilesets);
    }
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("Embed tileset data in map files.");
    }

    bool humanReadableOutput = Prefs::GetHumanReadableOutput();
    if (ImGui::Checkbox("Human-readable output", &humanReadableOutput))
    {
      Prefs::SetHumanReadableOutput(humanReadableOutput);
    }

    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "Human-readable saves are easier for humans to process, but take up "
          "more space.");
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Preferred format:");

    int formatIndex = (Prefs::GetPreferredFormat() == "JSON") ? 0 : 1;
    ImGui::SameLine();
    if (ImGui::Combo("##PreferredFormatCombo", &formatIndex, "JSON\0TMX\0\0"))
    {
      Prefs::SetPreferredFormat((formatIndex == 0) ? "JSON" : "TMX");
    }

    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("The default save file format.");
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Tiled compatibility");
    ImGui::SameLine();
    HelpMarker("Settings only relevant for Tiled compatibility");

    ImGui::Checkbox("Generate default values", &generate_default_values);

    static int tileWidth = 32;
    static int tileHeight = 32;

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Tile width: ");
    ImGui::SameLine();
    ImGui::InputInt("##TileWidthInput", &tileWidth);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Tile height:");
    ImGui::SameLine();
    ImGui::InputInt("##TileHeightInput", &tileHeight);

    ImGui::EndTabItem();
  }
}

}  // namespace

void UpdateSettingsDialog(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{500, 300});

  if (ImGui::Begin("Settings", open, ImGuiWindowFlags_NoCollapse))
  {
    if (ImGui::BeginTabBar("SettingsTabBar"))
    {
      ShowGeneralTab();
      ShowThemeBar();
      ShowExportTab();
      ImGui::EndTabBar();
    }
  }

  ImGui::PopStyleVar();
  ImGui::End();
}

}  // namespace Tactile
