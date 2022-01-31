#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/centered_button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "tileset_tabs.hpp"

namespace tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

}  // namespace

void TilesetDock::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  ResetState();

  auto& prefs = get_preferences();
  bool visible = prefs.is_tileset_dock_visible();

  if (!visible) {
    return;
  }

  scoped::Window window{"Tilesets", gWindowFlags, &visible};
  if (window.IsOpen()) {
    mHasFocus = ImGui::IsWindowFocused();
    mWindowContainsMouse = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

    if (!registry.view<comp::tileset>().empty()) {
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

  prefs.set_tileset_dock_visible(visible);
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

}  // namespace tactile