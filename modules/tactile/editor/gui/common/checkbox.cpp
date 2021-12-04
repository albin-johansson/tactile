#include "checkbox.hpp"

#include <cassert>  // assert

#include <imgui.h>

namespace Tactile {

auto Checkbox(const NotNull<CStr> label, NotNull<bool*> value, const CStr tooltip) -> bool
{
  assert(label);
  assert(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("%s", tooltip);
  }

  return changed;
}

}  // namespace Tactile
