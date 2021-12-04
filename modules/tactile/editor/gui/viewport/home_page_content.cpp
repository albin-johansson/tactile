#include "home_page_content.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/events/map_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/centered_button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/texture_utils.hpp"

namespace Tactile {

void UpdateHomePageContent(const Icons& icons, entt::dispatcher& dispatcher)
{
  PrepareVerticalAlignmentCenter(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  AlignNextItemCenteredHorizontally(128);
  ImGui::Image(ToTextureID(icons.GetTactileIcon()), {128, 128});

  CenteredText("No active map");

  ImGui::Spacing();
  if (CenteredButton("Create new map")) {
    dispatcher.enqueue<ShowNewMapDialogEvent>();
  }

  ImGui::Spacing();
  if (CenteredButton("Open existing map")) {
    dispatcher.enqueue<ShowOpenMapDialogEvent>();
  }
}

}  // namespace Tactile
