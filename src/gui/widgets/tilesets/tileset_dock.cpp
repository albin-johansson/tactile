#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "events/tileset_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "io/preferences.hpp"
#include "tileset_content_widget.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;

}  // namespace

void UpdateTilesetDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowTilesetDock()) {
    return;
  }

  bool isVisible = Prefs::GetShowTilesetDock();
  if (ImGui::Begin("Tilesets",
                   &isVisible,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
  {
    has_focus = ImGui::IsWindowFocused();

    if (!registry.empty<Tileset>()) {
      TilesetContentWidget(registry, dispatcher);
    }
    else {
      PrepareVerticalAlignmentCenter(2);

      CenteredText("No available tilesets!");
      if (CenteredButton(TAC_ICON_TILESET " Create tileset...")) {
        ShowTilesetDialog();
      }
    }
  }
  else {
    has_focus = false;
  }

  Prefs::SetShowTilesetDock(isVisible);
  ImGui::End();
}

auto IsTilesetDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
