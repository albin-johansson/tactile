#include "checkbox.hpp"

#include <imgui.h>

#include "misc/assert.hpp"

namespace tactile {

auto checkbox(const char* label, bool* value, const char* tooltip) -> bool
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
