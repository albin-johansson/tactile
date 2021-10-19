#include "string_property_widget.hpp"

#include <array>   // array
#include <limits>  // numeric_limits

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

#include "core/utils/buffer_utils.hpp"
#include "core/utils/scope_id.hpp"

namespace Tactile {
namespace {

constexpr usize max_digits = 100;

}  // namespace

auto StringPropertyWidget(const PropertyValue& property) -> Maybe<std::string>
{
  const ScopeID id{&property};
  const auto& str = property.AsString();

  std::array<char, max_digits> buffer;  // NOLINT safely uninitialized
  CopyStringIntoBuffer(buffer, str);

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  if (ImGui::InputTextWithHint("##StringPropertyInput",
                               "N/A",
                               buffer.data(),
                               sizeof buffer))
  {
    return CreateStringFromBuffer(buffer);
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[string]");
  }

  return nothing;
}

}  // namespace Tactile
