#include "combo.hpp"

#include <cassert>  // assert

#include <imgui.h>

#include "editor/gui/scoped.hpp"

namespace Tactile {

auto Combo(const NotNull<CStr> label,
           const NotNull<CStr> values,
           NotNull<int*> index,
           const CStr tooltip) -> bool
{
  assert(label);
  assert(values);
  assert(index);

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(label);
  ImGui::SameLine();

  const Scoped::ID scope{label};
  const auto changed = ImGui::Combo("##TactileCombo", index, values);

  if (tooltip && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("%s", tooltip);
  }

  return changed;
}

}  // namespace Tactile
