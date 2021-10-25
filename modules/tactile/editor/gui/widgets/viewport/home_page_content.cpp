#include "home_page_content.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/texture_utils.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/centered_button.hpp"
#include "editor/gui/widgets/common/centered_text.hpp"
#include "editor/gui/widgets/menus/file_menu.hpp"

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
