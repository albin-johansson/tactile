#include "settings_dialog.hpp"

#include <imgui.h>

#include "gui/widgets/help_marker.hpp"
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
    static int currentItem = 0;
    ImGui::Combo("Theme", &currentItem, "Dark\0Light\0\0");

    ImGui::EndTabItem();
  }
}

void ShowExportTab()
{
  if (ImGui::BeginTabItem("Export"))
  {
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

    int formatIndex = (Prefs::GetPreferredFormat() == "JSON") ? 0 : 1;
    if (ImGui::Combo("Default format", &formatIndex, "JSON\0TMX\0\0"))
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
    ImGui::InputInt("Tile width", &tileWidth);
    ImGui::InputInt("Tile height", &tileHeight);

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
