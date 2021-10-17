#include "view_menu.hpp"

#include <imgui.h>

#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layout/dock_space.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

void UpdateWidgetsSubmenu(const bool hasActiveMap)
{
  if (auto menu = Menu{"Widgets", hasActiveMap}) {
    if (ImGui::MenuItem("Reset Layout")) {
      ResetLayout();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Properties", nullptr, Prefs::GetShowPropertiesDock())) {
      Prefs::SetShowPropertiesDock(!Prefs::GetShowPropertiesDock());
    }

    if (ImGui::MenuItem("Layers", nullptr, Prefs::GetShowLayerDock())) {
      Prefs::SetShowLayerDock(!Prefs::GetShowLayerDock());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, Prefs::GetShowTilesetDock())) {
      Prefs::SetShowTilesetDock(!Prefs::GetShowTilesetDock());
    }
  }
}

}  // namespace

void UpdateViewMenu(const Model& model, entt::dispatcher& dispatcher)
{
  if (auto menu = Menu{"View"}) {
    const auto hasActiveDocument = model.HasActiveDocument();
    UpdateWidgetsSubmenu(hasActiveDocument);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center Viewport",
                        "Ctrl+Space",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (bool showGrid = Prefs::GetShowGrid();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid", "Ctrl+G", &showGrid))
    {
      Prefs::SetShowGrid(showGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_ZOOM_IN " Increase Zoom",
                        "Ctrl+Plus",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_ZOOM_OUT " Decrease Zoom",
                        "Ctrl+Minus",
                        false,
                        model.CanDecreaseViewportTileSize()))
    {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset Zoom",
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Pan Up", "Ctrl+Up", false, hasActiveDocument))
    {
      dispatcher.enqueue<PanUpEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Pan Down",
                        "Ctrl+Down",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanDownEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_RIGHT " Pan Right",
                        "Ctrl+Right",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanRightEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_LEFT " Pan Left",
                        "Ctrl+Left",
                        false,
                        hasActiveDocument))
    {
      dispatcher.enqueue<PanLeftEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Toggle UI", "Tab", false, hasActiveDocument)) {
      dispatcher.enqueue<ToggleUiEvent>();
    }
  }
}

}  // namespace Tactile
