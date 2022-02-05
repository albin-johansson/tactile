#include "checkbox.hpp"

#include <imgui.h>

#include "misc/assert.hpp"

namespace tactile {

auto Checkbox(const c_str label, bool* value, const c_str tooltip) -> bool
{
  TACTILE_ASSERT(label);
  TACTILE_ASSERT(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("%s", tooltip);
  }

  return changed;
}

}  // namespace tactile
