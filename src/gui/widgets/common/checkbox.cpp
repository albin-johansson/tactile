#include "checkbox.hpp"

#include <imgui.h>

#include <cassert>  // assert

namespace Tactile {

auto Checkbox(const NotNull<CStr> label,
              NotNull<bool*> value,
              const czstring tooltip) -> bool
{
  assert(label);
  assert(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip && ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("%s", tooltip);
  }

  return changed;
}

}  // namespace Tactile
