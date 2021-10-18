#include "home_page_content.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/editor/gui/icons.hpp"
#include "tactile/editor/gui/texture_utils.hpp"
#include "tactile/editor/gui/widgets/alignment.hpp"
#include "tactile/editor/gui/widgets/common/centered_button.hpp"
#include "tactile/editor/gui/widgets/common/centered_text.hpp"
#include "tactile/editor/gui/widgets/menus/file_menu.hpp"

namespace Tactile {

void UpdateHomePageContent()
{
  PrepareVerticalAlignmentCenter(4);

  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2{0, 64});

  AlignNextItemCenteredHorizontally(128);
  ImGui::Image(ToTextureID(GetTactileIcon()), {128, 128});

  CenteredText("No active map");

  ImGui::Spacing();
  if (CenteredButton("Create new map")) {
    ShowAddMapDialog();
  }

  ImGui::Spacing();
  if (CenteredButton("Open existing map")) {
    ShowOpenMapDialog();
  }
}

}  // namespace Tactile
