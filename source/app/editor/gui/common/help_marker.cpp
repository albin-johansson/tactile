#include "help_marker.hpp"

#include <imgui.h>

#include "misc/assert.hpp"

namespace tactile {

void HelpMarker(const c_str text)
{
  TACTILE_ASSERT(text);

  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();

    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(text);
    ImGui::PopTextWrapPos();

    ImGui::EndTooltip();
  }
}

}  // namespace tactile
