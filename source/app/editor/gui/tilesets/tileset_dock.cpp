#include "tileset_dock.hpp"

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/scoped.hpp"
#include "io/persistence/preferences.hpp"
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

  scoped::window window{"Tilesets", gWindowFlags, &visible};
  if (window.is_open()) {
    mHasFocus = ImGui::IsWindowFocused();
    mWindowContainsMouse = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);

    if (!registry.view<comp::tileset>().empty()) {
      mTabWidget.Update(registry, dispatcher);
    }
    else {
      prepare_vertical_alignment_center(2);
      centered_text("Current map has no tilesets!");
      ImGui::Spacing();
      if (centered_button(TAC_ICON_TILESET " Create tileset...")) {
        dispatcher.enqueue<show_add_tileset_dialog_event>();
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
