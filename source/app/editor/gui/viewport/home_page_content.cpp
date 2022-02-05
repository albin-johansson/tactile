#include "home_page_content.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/textures.hpp"

namespace tactile {

void UpdateHomePageContent(const icon_manager& icons, entt::dispatcher& dispatcher)
{
  prepare_vertical_alignment_center(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  center_next_item_horizontally(128);
  ImGui::Image(to_texture_id(icons.tactile_icon()), {128, 128});

  ImGui::Spacing();
  ImGui::Spacing();

  if (centered_button("Create new map")) {
    dispatcher.enqueue<show_new_map_dialog_event>();
  }

  ImGui::Spacing();
  if (centered_button("Open existing map")) {
    dispatcher.enqueue<show_open_map_dialog_event>();
  }
}

}  // namespace tactile
