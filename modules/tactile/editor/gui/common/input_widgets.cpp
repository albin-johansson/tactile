#include "input_widgets.hpp"

#include <array>    // array
#include <limits>   // numeric_limits
#include <utility>  // move

#include "core/utils/buffer_utils.hpp"
#include "core/utils/color_utils.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/file_dialog.hpp"

namespace tactile {

auto Input(const CStr id, const attribute_value& value) -> Maybe<attribute_value>
{
  switch (value.type()) {
    case PropertyType::String: {
      if (auto updated = InputStringWithHint(id, "Empty", value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case PropertyType::Integer: {
      if (const auto updated = InputWidget(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case PropertyType::Floating: {
      if (const auto updated = InputWidget(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case PropertyType::Boolean: {
      if (const auto updated = InputWidget(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case PropertyType::File: {
      if (auto updated = InputFile(id, value.as_file())) {
        return std::move(updated);
      }
      break;
    }
    case PropertyType::Color: {
      if (const auto updated = InputWidget(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case PropertyType::Object: {
      if (const auto updated = InputWidget(id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto InputWidget(const CStr id, int value) -> Maybe<int>
{
  const scoped::ID scope{id};

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
  const scoped::ID scope{id};

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
  const scoped::ID scope{id};

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
  const scoped::ID scope{id};

  if (ImGui::Checkbox("##InputString[bool]", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[bool]");
  }

  return nothing;
}

auto InputWidget(const CStr id, const object_t value) -> Maybe<object_t>
{
  const scoped::ID scope{id};

  // TODO
  ImGui::Text("%i", value);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

auto InputWidget(const CStr id, const cen::color value) -> Maybe<cen::color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const scoped::ID scope{id};

  auto arr = ColorToArray(value);
  if (ImGui::ColorEdit4("##InputString[color]", arr.data(), flags)) {
    return cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

auto InputFile(const CStr id, const std::filesystem::path& value)
    -> Maybe<std::filesystem::path>
{
  const scoped::ID scope{id};

  if (ImGui::Button(TAC_ICON_THREE_DOTS)) {
    auto dialog = FileDialog::OpenFile();
    if (dialog.IsOkay()) {
      return dialog.GetPath();
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

}  // namespace tactile
