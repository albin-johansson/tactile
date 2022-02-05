#include "view_menu.hpp"

#include <imgui.h>

#include "editor/events/misc_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layout/dock_space.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "editor/shortcuts/mappings.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {
namespace {

void UpdateWidgetsMenu(const bool hasActiveMap)
{
  if (scoped::Menu menu{"Widgets", hasActiveMap}; menu.IsOpen()) {
    if (ImGui::MenuItem("Reset Layout")) {
      ResetLayout();
    }

    ImGui::Separator();

    auto& prefs = get_preferences();

    if (ImGui::MenuItem("Properties", nullptr, prefs.is_properties_dock_visible())) {
      prefs.set_properties_dock_visible(!prefs.is_properties_dock_visible());
    }

    if (ImGui::MenuItem("Layers", nullptr, prefs.is_layer_dock_visible())) {
      prefs.set_layer_dock_visible(!prefs.is_layer_dock_visible());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, prefs.is_tileset_dock_visible())) {
      prefs.set_tileset_dock_visible(!prefs.is_tileset_dock_visible());
    }

    if (ImGui::MenuItem("Log", nullptr, prefs.is_log_dock_visible())) {
      prefs.set_log_dock_visible(!prefs.is_log_dock_visible());
    }

    if (ImGui::MenuItem("Components", nullptr, prefs.is_component_dock_visible())) {
      prefs.set_component_dock_visible(!prefs.is_component_dock_visible());
    }
  }
}

}  // namespace

void ViewMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (scoped::Menu menu{"View"}; menu.IsOpen()) {
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

    auto& prefs = get_preferences();

    if (bool visible = prefs.is_grid_visible();
        ImGui::MenuItem(TAC_ICON_GRID " Toggle Grid",
                        TACTILE_PRIMARY_MOD "+G",
                        &visible)) {
      prefs.set_grid_visible(visible);
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
      dispatcher.enqueue<toggle_ui_event>();
    }
  }
}

}  // namespace tactile
