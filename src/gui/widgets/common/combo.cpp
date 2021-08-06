#include "combo.hpp"

#include <imgui.h>

#include "utils/scope_id.hpp"

namespace Tactile {

auto Combo(const czstring label,
           const czstring values,
           NotNull<int*> index,
           const czstring tooltip) -> bool
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
  ImGui::SameLine();

  const ScopeID uid{label};
  const auto changed = ImGui::Combo("##TactileCombo", index, values);

  if (tooltip && ImGui::IsItemHovered())
  {
    ImGui::SetTooltip(tooltip);
  }

  return changed;
}

}  // namespace Tactile
