#include "view_menu.hpp"

#include <imgui.h>

#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layout/dock_space.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/preferences.hpp"

namespace Tactile {

void ViewMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (Scoped::Menu menu{"View"}; menu.IsOpen()) {
    const auto hasActiveDocument = model.HasActiveDocument();
    UpdateWidgetsMenu(hasActiveDocument);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center Viewport",
                        "Shift+Space",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    ImGui::Separator();

    if (bool showGrid = Prefs::GetShowGrid();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid",
                        TACTILE_PRIMARY_MOD "+G",
                        &showGrid)) {
      Prefs::SetShowGrid(showGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_ZOOM_IN " Increase Zoom",
                        TACTILE_PRIMARY_MOD "+Plus",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<IncreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_ZOOM_OUT " Decrease Zoom",
                        TACTILE_PRIMARY_MOD "+Minus",
                        false,
                        model.CanDecreaseViewportTileSize())) {
      dispatcher.enqueue<DecreaseZoomEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset Zoom",
                        nullptr,
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<ResetZoomEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Pan Up",
                        TACTILE_PRIMARY_MOD "+Shift+Up",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanUpEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Pan Down",
                        TACTILE_PRIMARY_MOD "+Shift+Down",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanDownEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_RIGHT " Pan Right",
                        TACTILE_PRIMARY_MOD "+Shift+Right",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanRightEvent>();
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_LEFT " Pan Left",
                        TACTILE_PRIMARY_MOD "+Shift+Left",
                        false,
                        hasActiveDocument)) {
      dispatcher.enqueue<PanLeftEvent>();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Toggle UI", "Tab", false, hasActiveDocument)) {
      dispatcher.enqueue<ToggleUiEvent>();
    }
  }
}

void ViewMenu::UpdateWidgetsMenu(const bool hasActiveMap)
{
  if (Scoped::Menu menu{"Widgets", hasActiveMap}; menu.IsOpen()) {
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

    if (ImGui::MenuItem("Log", nullptr, Prefs::GetShowLogDock())) {
      Prefs::SetShowLogDock(!Prefs::GetShowLogDock());
    }

    if (ImGui::MenuItem("Components", nullptr, Prefs::GetShowComponentDock())) {
      Prefs::SetShowComponentDock(!Prefs::GetShowComponentDock());
    }
  }
}

}  // namespace Tactile
