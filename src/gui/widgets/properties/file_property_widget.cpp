#include "file_property_widget.hpp"

#include <imgui.h>

#include "utils/scope_id.hpp"

namespace Tactile {

auto FilePropertyWidget(const Property& property)
    -> Maybe<std::filesystem::path>
{
  const ScopeID id{&property};

  const auto& path = property.AsFile();
  const auto str = path.filename().string();
  ImGui::TextUnformatted(str.c_str());

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[file]");
  }

  return nothing;
}

}  // namespace Tactile
