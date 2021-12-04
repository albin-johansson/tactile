#include "help_marker.hpp"

#include <cassert>  // assert

#include <imgui.h>

namespace Tactile {

void HelpMarker(const NotNull<CStr> text)
{
  assert(text);

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
