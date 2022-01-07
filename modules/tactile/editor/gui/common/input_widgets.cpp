#include "input_widgets.hpp"

#define NOMINMAX

#include <array>   // array
#include <limits>  // numeric_limits
#include <utility>  // move

#include <portable-file-dialogs.h>

#include "core/utils/buffer_utils.hpp"
#include "core/utils/color_utils.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

auto Input(const CStr id, const PropertyValue& value) -> Maybe<PropertyValue>
{
  switch (value.GetType().value()) {
    case PropertyType::String: {
      if (auto updated = InputStringWithHint(id, "Empty", value.AsString())) {
        return std::move(updated);
      }
      break;
    }
    case PropertyType::Integer: {
      if (const auto updated = InputWidget(id, value.AsInt())) {
        return updated;
      }
      break;
    }
    case PropertyType::Floating: {
      if (const auto updated = InputWidget(id, value.AsFloat())) {
        return updated;
      }
      break;
    }
    case PropertyType::Boolean: {
      if (const auto updated = InputWidget(id, value.AsBool())) {
        return updated;
      }
      break;
    }
    case PropertyType::File: {
      if (auto updated = InputFile(id, value.AsFile())) {
        return std::move(updated);
      }
      break;
    }
    case PropertyType::Color: {
      if (const auto updated = InputWidget(id, value.AsColor())) {
        return updated;
      }
      break;
    }
    case PropertyType::Object: {
      if (const auto updated = InputWidget(id, value.AsObject())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto InputWidget(const CStr id, int value) -> Maybe<int>
{
  const Scoped::ID scope{id};

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  if (ImGui::DragInt("##InputString[int]", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[int]");
  }

  return nothing;
}

auto InputWidget(const CStr id, float value, const float min, const float max)
    -> Maybe<float>
{
  const Scoped::ID scope{id};

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat("##InputString[float]", &value, min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat("##InputString[float]", &value)) {
      return value;
    }
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[float]");
  }

  return nothing;
}

auto InputStringWithHint(const CStr id,
                         const CStr hint,
                         const std::string& value,
                         const CStr label,
                         const ImGuiInputTextFlags flags,
                         const ImGuiInputTextCallback filter) -> Maybe<std::string>
{
  const Scoped::ID scope{id};

  std::array<char, 100> buffer;  // NOLINT safely uninitialized
  CopyStringIntoBuffer(buffer, value);

  if (label) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);

    ImGui::SameLine();
  }
  else {
    ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  }

  if (ImGui::InputTextWithHint("##InputString[string]",
                               hint ? hint : "",
                               buffer.data(),
                               sizeof buffer,
                               flags,
                               filter)) {
    return CreateStringFromBuffer(buffer);
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[string]");
  }

  return nothing;
}

auto InputString(const CStr id,
                 const std::string& value,
                 const CStr label,
                 const ImGuiInputTextFlags flags,
                 const ImGuiInputTextCallback filter) -> Maybe<std::string>
{
  return InputStringWithHint(id, nullptr, value, label, flags, filter);
}

auto InputWidget(const CStr id, bool value) -> Maybe<bool>
{
  const Scoped::ID scope{id};

  if (ImGui::Checkbox("##InputString[bool]", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[bool]");
  }

  return nothing;
}

auto InputWidget(const CStr id, const ObjectRef value) -> Maybe<ObjectRef>
{
  const Scoped::ID scope{id};

  // TODO
  ImGui::Text("%i", value);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

auto InputWidget(const CStr id, const cen::Color value) -> Maybe<cen::Color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const Scoped::ID scope{id};

  auto arr = ColorToArray(value);
  if (ImGui::ColorEdit4("##InputString[color]", arr.data(), flags)) {
    return cen::Color::FromNorm(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

auto InputFile(const CStr id, const std::filesystem::path& value)
    -> Maybe<std::filesystem::path>
{
  const Scoped::ID scope{id};

  if (ImGui::Button(TAC_ICON_THREE_DOTS)) {
    auto files = pfd::open_file{"Select File..."}.result();
    if (!files.empty()) {
      return files.front();
    }
  }

  ImGui::SameLine();

  auto str = value.filename().string();

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  ImGui::InputTextWithHint("##InputString[file]",
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
