#include "help_marker.hpp"

#include <imgui.h>

#include "assert.hpp"

namespace Tactile {

void HelpMarker(const NotNull<CStr> text)
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

}  // namespace Tactile
