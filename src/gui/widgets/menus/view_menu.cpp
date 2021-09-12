#include "view_menu.hpp"

#include <imgui.h>

#include "core/model.hpp"
#include "events/view_events.hpp"
#include "events/viewport_events.hpp"
#include "gui/icons.hpp"
#include "gui/layout/dock_space.hpp"
#include "gui/widgets/toolbar/toolbar.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

void UpdateWidgetsSubmenu(const bool hasActiveMap, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("Widgets", hasActiveMap))
  {
    if (ImGui::MenuItem("Reset layout"))
    {
      ResetLayout();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Properties", nullptr, Prefs::GetShowPropertiesDock()))
    {
      Prefs::SetShowPropertiesDock(!Prefs::GetShowPropertiesDock());
    }

    if (ImGui::MenuItem("Layers", nullptr, Prefs::GetShowLayerDock()))
    {
      Prefs::SetShowLayerDock(!Prefs::GetShowLayerDock());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, Prefs::GetShowTilesetDock()))
    {
      Prefs::SetShowTilesetDock(!Prefs::GetShowTilesetDock());
    }

    ImGui::EndMenu();
  }
}

}  // namespace

void UpdateViewMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto hasActiveDocument = model.HasActiveDocument();

  if (ImGui::BeginMenu("View"))
  {
    UpdateWidgetsSubmenu(hasActiveDocument, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center viewport",
                        "Ctrl+Space",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (bool showGrid = Prefs::GetShowGrid();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle grid", "Ctrl+G", &showGrid))
    {
      Prefs::SetShowGrid(showGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_ZOOM_IN " Increase zoom",
                        "Ctrl+Plus",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_ZOOM_OUT " Decrease zoom",
                        "Ctrl+Minus",
                        false,
                        model.CanDecreaseViewportTileSize()))
    {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset zoom",
                        nullptr,
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Pan up",
                        "Ctrl+Up",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanUpEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Pan down",
                        "Ctrl+Down",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanDownEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_RIGHT " Pan right",
                        "Ctrl+Right",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanRightEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_LEFT " Pan left",
                        "Ctrl+Left",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanLeftEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Toggle UI", "Tab", false, hasActiveDocument))
    {
      dispatcher.enqueue<ToggleUiEvent>();
    }

    ImGui::EndMenu();
  }
}

}  // namespace Tactile
