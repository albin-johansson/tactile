#include "file_property_widget.hpp"

#define NOMINMAX

#include <limits>  // numeric_limits

#include <imgui.h>

#include "tactile/core/utils/scope_id.hpp"
#include "tactile/editor/gui/icons.hpp"

#include <portable-file-dialogs.h>

namespace Tactile {

auto FilePropertyWidget(const PropertyValue& property) -> Maybe<std::filesystem::path>
{
  const ScopeID id{&property};

  if (ImGui::Button(TAC_ICON_SELECT_FILE)) {
    auto files = pfd::open_file{"Select File..."}.result();
    if (!files.empty()) {
      return files.front();
    }
  }

  const auto& path = property.AsFile();
  auto str = path.filename().string();

  ImGui::SameLine();
  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  ImGui::InputTextWithHint("##FilePropertyInput",
                           "N/A",
                           str.data(),
                           str.capacity(),
                           ImGuiInputTextFlags_ReadOnly);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[file]");
  }

  return nothing;
}

}  // namespace Tactile
