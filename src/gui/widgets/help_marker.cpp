#include "help_marker.hpp"

#include <imgui.h>

namespace Tactile {

void HelpMarker(const czstring text)
{
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered())
  {
    ImGui::BeginTooltip();

    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(text);
    ImGui::PopTextWrapPos();

    ImGui::EndTooltip();
  }
}

}  // namespace Tactile
