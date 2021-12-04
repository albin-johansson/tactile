#include "button.hpp"

#include <cassert>  // assert

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/gui/widgets/scoped.hpp"

namespace Tactile {

auto Button(const NotNull<CStr> text,
            const CStr tooltip,
            const bool enabled,
            const float width,
            const float height) -> bool
{
  assert(text);

  const Scoped::Disable disable{!enabled};

  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !enabled);

  const auto result = ImGui::Button(text, {width, height});

  if (tooltip) {
    if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
      ImGui::SetTooltip("%s", tooltip);
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
  }

  ImGui::PopItemFlag();

  return result;
}

}  // namespace Tactile
