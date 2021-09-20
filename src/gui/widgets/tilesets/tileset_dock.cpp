#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "io/preferences.hpp"
#include "tileset_tabs.hpp"

namespace Tactile {
namespace {

constinit bool has_focus = false;
constexpr auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

void UpdateTilesetDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowTilesetDock()) {
    return;
  }

  bool visible = Prefs::GetShowTilesetDock();
  if (ImGui::Begin("Tilesets", &visible, window_flags)) {
    has_focus = ImGui::IsWindowFocused();

    if (!registry.empty<Tileset>()) {
      UpdateTilesetTabs(registry, dispatcher);
    }
    else {
      PrepareVerticalAlignmentCenter(2);
      CenteredText("Current map has no tilesets!");
      ImGui::Spacing();
      if (CenteredButton(TAC_ICON_TILESET " Create tileset...")) {
        ShowTilesetDialog();
      }
    }
  }
  else {
    has_focus = false;
  }

  Prefs::SetShowTilesetDock(visible);
  ImGui::End();
}

auto IsTilesetDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
