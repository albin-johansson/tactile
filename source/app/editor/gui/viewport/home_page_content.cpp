#include "home_page_content.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/centered_button.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/texture_utils.hpp"

namespace tactile {

void UpdateHomePageContent(const icon_manager& icons, entt::dispatcher& dispatcher)
{
  PrepareVerticalAlignmentCenter(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  AlignNextItemCenteredHorizontally(128);
  ImGui::Image(ToTextureID(icons.tactile_icon()), {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  if (CenteredButton("Create new map")) {
    dispatcher.enqueue<show_new_map_dialog_event>();
  }

  ImGui::Spacing();
  if (CenteredButton("Open existing map")) {
    dispatcher.enqueue<show_open_map_dialog_event>();
  }
}

}  // namespace tactile
