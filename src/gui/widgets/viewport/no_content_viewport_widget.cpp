#include "no_content_viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/events/maps/add_map_event.hpp"
#include "core/events/maps/open_map_event.hpp"
#include "gui/icons.hpp"
#include "gui/texture_utils.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"

namespace Tactile {

void NoContentViewportWidget(entt::dispatcher& dispatcher)
{
  PrepareVerticalAlignmentCenter(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  AlignNextItemCenteredHorizontally(128);
  ImGui::Image(ToTextureID(GetTactileIcon()), {128, 128});

  CenteredText("No active map");

  ImGui::Spacing();
  if (CenteredButton("Create new map"))
  {
    dispatcher.enqueue<AddMapEvent>();
  }

  ImGui::Spacing();
  if (CenteredButton("Open existing map"))
  {
    dispatcher.enqueue<OpenMapEvent>();
  }
}

}  // namespace Tactile
