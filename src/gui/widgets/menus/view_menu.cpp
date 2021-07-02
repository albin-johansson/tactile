#include "view_menu.hpp"

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include "core/events/center_viewport_event.hpp"
#include "core/model.hpp"
#include "gui/layout/dock_space.hpp"
#include "gui/widgets/layers/layer_widget.hpp"
#include "gui/widgets/properties/properties_widget.hpp"
#include "gui/widgets/tilesets/tileset_widget.hpp"
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

    if (ImGui::MenuItem("Properties", nullptr, IsPropertiesWidgetVisible()))
    {
      SetPropertiesWidgetVisible(!IsPropertiesWidgetVisible());
    }

    if (ImGui::MenuItem("Layers", nullptr, IsLayerWidgetVisible()))
    {
      SetLayerWidgetVisible(!IsLayerWidgetVisible());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, IsTilesetWidgetVisible()))
    {
      SetTilesetWidgetVisible(!IsTilesetWidgetVisible());
    }

    ImGui::EndMenu();
  }
}

}  // namespace

void UpdateViewMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto hasActiveMap = model.GetActiveMapId().has_value();

  if (ImGui::BeginMenu("View"))
  {
    UpdateWidgetsSubmenu(hasActiveMap, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_CROSSHAIRS " Center viewport",
                        "Ctrl+Space",
                        false,
                        hasActiveMap))
    {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    bool showGrid = Prefs::GetShowGrid();
    if (ImGui::MenuItem(ICON_FA_TH " Toggle grid", "Ctrl+G", &showGrid))
    {
      Prefs::SetShowGrid(showGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_SEARCH_PLUS " Increase zoom",
                        "Ctrl+Plus",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(ICON_FA_SEARCH_MINUS " Decrease zoom",
                        "Ctrl+Minus",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(ICON_FA_SEARCH " Reset zoom",
                        nullptr,
                        false,
                        hasActiveMap))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem(ICON_FA_ARROW_UP " Pan up",
                        "Ctrl+Up",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_DOWN " Pan down",
                        "Ctrl+Down",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_RIGHT " Pan right",
                        "Ctrl+Right",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(ICON_FA_ARROW_LEFT " Pan left",
                        "Ctrl+Left",
                        false,
                        hasActiveMap))
    {}

    ImGui::EndMenu();
  }
}

}  // namespace Tactile
