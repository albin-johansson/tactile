#include "no_content_viewport_widget.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "gui/icons.hpp"
#include "gui/texture_utils.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/centered_button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "gui/widgets/menus/file_menu.hpp"

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
    EnableAddMapDialog();
  }

  ImGui::Spacing();
  if (CenteredButton("Open existing map"))
  {
    EnableOpenMapDialog();
  }
}

}  // namespace Tactile
