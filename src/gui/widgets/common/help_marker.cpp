#include "help_marker.hpp"

#include <imgui.h>

#include <cassert>  // assert

namespace Tactile {

void HelpMarker(const NotNull<czstring> text)
{
  assert(text);

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
