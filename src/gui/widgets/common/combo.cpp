#include "combo.hpp"

#include <imgui.h>

#include <cassert>  // assert

#include "utils/scope_id.hpp"

namespace Tactile {

auto Combo(const NotNull<czstring> label,
           const NotNull<czstring> values,
           NotNull<int*> index,
           const czstring tooltip) -> bool
{
  assert(label);
  assert(values);
  assert(index);

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
