#include "checkbox.hpp"

#include <imgui.h>

#include "assert.hpp"

namespace Tactile {

auto Checkbox(const NotNull<CStr> label, NotNull<bool*> value, const CStr tooltip) -> bool
{
  TACTILE_ASSERT(label);
  TACTILE_ASSERT(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("%s", tooltip);
  }

  return changed;
}

}  // namespace Tactile
