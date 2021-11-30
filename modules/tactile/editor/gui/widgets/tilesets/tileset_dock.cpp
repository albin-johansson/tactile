#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/centered_button.hpp"
#include "editor/gui/widgets/common/centered_text.hpp"
#include "editor/gui/widgets/menus/map_menu.hpp"
#include "io/preferences.hpp"
#include "tileset_tabs.hpp"

namespace Tactile {
namespace {

constinit bool gHasFocus = false;
constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

void UpdateTilesetDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowTilesetDock()) {
    return;
  }

  bool visible = Prefs::GetShowTilesetDock();
  if (ImGui::Begin("Tilesets", &visible, gWindowFlags)) {
    gHasFocus = ImGui::IsWindowFocused();

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
    gHasFocus = false;
  }

  Prefs::SetShowTilesetDock(visible);
  ImGui::End();
}

auto IsTilesetDockFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile
