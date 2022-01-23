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

namespace tactile {
namespace {

void UpdateWidgetsMenu(const bool hasActiveMap)
{
  if (Scoped::Menu menu{"Widgets", hasActiveMap}; menu.IsOpen()) {
    if (ImGui::MenuItem("Reset Layout")) {
      ResetLayout();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Properties", nullptr, prefs::GetShowPropertiesDock())) {
      prefs::SetShowPropertiesDock(!prefs::GetShowPropertiesDock());
    }

    if (ImGui::MenuItem("Layers", nullptr, prefs::GetShowLayerDock())) {
      prefs::SetShowLayerDock(!prefs::GetShowLayerDock());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, prefs::GetShowTilesetDock())) {
      prefs::SetShowTilesetDock(!prefs::GetShowTilesetDock());
    }

    if (ImGui::MenuItem("Log", nullptr, prefs::GetShowLogDock())) {
      prefs::SetShowLogDock(!prefs::GetShowLogDock());
    }

    if (ImGui::MenuItem("Components", nullptr, prefs::GetShowComponentDock())) {
      prefs::SetShowComponentDock(!prefs::GetShowComponentDock());
    }
  }
}

}  // namespace

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

    if (bool showGrid = prefs::GetShowGrid();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid",
                        TACTILE_PRIMARY_MOD "+G",
                        &showGrid)) {
      prefs::SetShowGrid(showGrid);
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

}  // namespace tactile
