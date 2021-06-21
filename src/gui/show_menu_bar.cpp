#include "show_menu_bar.hpp"

#include "imgui.h"
#include "show_map_viewport.hpp"

namespace tactile {
namespace {

inline bool show_about_window = false;
inline bool show_metrics_window = false;

void ShowFileMenu()
{
  if (ImGui::BeginMenu("File"))
  {
    if (ImGui::MenuItem("New map...", "CTRL+N"))
    {}

    if (ImGui::MenuItem("Open map...", "CTRL+O"))
    {}

    if (ImGui::MenuItem("Close map"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Save", "CTRL+S"))
    {}

    if (ImGui::MenuItem("Save as...", "CTRL+SHIFT+S"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Settings...", "CTRL+SHIFT+S"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Exit"))
    {}

    ImGui::EndMenu();
  }
}

void ShowEditMenu()
{
  if (ImGui::BeginMenu("Edit"))
  {
    if (ImGui::MenuItem("Undo", "CTRL+Z"))
    {}

    if (ImGui::MenuItem("Redo", "CTRL+Y"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Add row", "ALT+R"))
    {}

    if (ImGui::MenuItem("Add column", "ALT+C"))
    {}

    if (ImGui::MenuItem("Remove row", "ALT+SHIFT+R"))
    {}

    if (ImGui::MenuItem("Remove column", "ALT+SHIFT+C"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Stamp", "S"))
    {}

    if (ImGui::MenuItem("Bucket", "B"))
    {}

    if (ImGui::MenuItem("Eraser", "E"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Add tileset...", "CTRL+T"))
    {}

    ImGui::EndMenu();
  }
}

void ShowViewAppearanceSubmenu()
{
  if (ImGui::BeginMenu("Appearance"))
  {
    if (ImGui::MenuItem("Mouse tools", nullptr, true))
    {}

    if (ImGui::MenuItem("Properties", nullptr, true))
    {}

    if (ImGui::MenuItem("Layers", nullptr, true))
    {}

    if (ImGui::MenuItem("Tilesets", nullptr, true))
    {}

    ImGui::EndMenu();
  }
}

void ShowViewMenu()
{
  if (ImGui::BeginMenu("View"))
  {
    ShowViewAppearanceSubmenu();

    ImGui::Separator();

    if (ImGui::MenuItem("Center viewport", "CTRL+SPACE"))
    {}

    static bool isGridItemToggled = true;
    if (ImGui::MenuItem("Toggle grid", "CTRL+G", &isGridItemToggled))
    {
      SetMapViewportGridEnabled(isGridItemToggled);
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Increase zoom", "CTRL++"))
    {}

    if (ImGui::MenuItem("Decrease zoom", "CTRL+-"))
    {}

    if (ImGui::MenuItem("Reset zoom"))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem("Pan up", "CTRL+UP"))
    {}

    if (ImGui::MenuItem("Pan down", "CTRL+DOWN"))
    {}

    if (ImGui::MenuItem("Pan right", "CTRL+RIGHT"))
    {}

    if (ImGui::MenuItem("Pan left", "CTRL+LEFT"))
    {}

    ImGui::EndMenu();
  }
}

void ShowHelpMenu()
{
  if (ImGui::BeginMenu("Help"))
  {
    ImGui::MenuItem("About Tactile...");
    show_about_window = ImGui::MenuItem("About ImGui...");

    ImGui::Separator();

    show_metrics_window = ImGui::MenuItem("Show metrics...");

    ImGui::EndMenu();
  }
}

}  // namespace

void ShowMenuBar()
{
  if (ImGui::BeginMainMenuBar())
  {
    ShowFileMenu();
    ShowEditMenu();
    ShowViewMenu();
    ShowHelpMenu();

    ImGui::EndMainMenuBar();
  }

  if (show_about_window)
  {
    ImGui::ShowAboutWindow(&show_about_window);
  }

  if (show_metrics_window)
  {
    ImGui::ShowMetricsWindow(&show_metrics_window);
  }
}

}  // namespace tactile
