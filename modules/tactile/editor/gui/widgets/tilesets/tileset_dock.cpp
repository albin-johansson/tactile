#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/centered_button.hpp"
#include "editor/gui/widgets/common/centered_text.hpp"
#include "editor/gui/widgets/common/window.hpp"
#include "editor/gui/widgets/menus/map_menu.hpp"
#include "io/preferences.hpp"
#include "tileset_tabs.hpp"

namespace Tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

void TilesetDock::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  ResetState();

  if (!Prefs::GetShowTilesetDock()) {
    return;
  }

  bool visible = Prefs::GetShowTilesetDock();
  Window window{"Tilesets", gWindowFlags, &visible};
  if (window.IsOpen()) {
    mHasFocus = ImGui::IsWindowFocused();
    mWindowContainsMouse = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

    if (!registry.empty<Tileset>()) {
      mTabWidget.Update(registry, dispatcher);
    }
    else {
      PrepareVerticalAlignmentCenter(2);
      CenteredText("Current map has no tilesets!");
      ImGui::Spacing();
      if (CenteredButton(TAC_ICON_TILESET " Create tileset...")) {
        dispatcher.enqueue<ShowAddTilesetDialogEvent>();
      }
    }
  }

  Prefs::SetShowTilesetDock(visible);
}

auto TilesetDock::GetTilesetView() const -> const TilesetView&
{
  return mTabWidget.GetTilesetView();
}

void TilesetDock::ResetState()
{
  mHasFocus = false;
  mWindowContainsMouse = false;
}

}  // namespace Tactile
