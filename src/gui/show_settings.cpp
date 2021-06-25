#include "show_settings.hpp"

#include "imgui.h"
#include "widgets/help_marker.hpp"

namespace tactile {
namespace {

inline bool restore_last_session = false;
inline bool rmb_stamp_as_eraser = false;
inline bool embed_tilesets = false;
inline bool human_readable_output = true;
inline bool generate_default_values = true;

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
    ImGui::Checkbox("Embed tilesets", &embed_tilesets);
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("Embed tileset data in map files.");
    }

    ImGui::Checkbox("Human-readable output", &human_readable_output);
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
          "Human-readable saves are easier for humans to process, but take up "
          "more space.");
    }

    static int currentItem = 0;
    ImGui::Combo("Default format", &currentItem, "JSON\0TMX\0\0");
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

void ShowSettings(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{500, 300});

  if (ImGui::Begin("Settings", open))
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

}  // namespace tactile
