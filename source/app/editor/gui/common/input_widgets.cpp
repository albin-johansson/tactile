#include "input_widgets.hpp"

#include <array>    // array
#include <utility>  // move

#include "core/utils/buffers.hpp"
#include "core/utils/colors.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/file_dialog.hpp"

namespace tactile {

auto Input(const c_str id, const attribute_value& value) -> maybe<attribute_value>
{
  switch (value.type()) {
    case attribute_type::string: {
      if (auto updated = InputStringWithHint(id, "Empty", value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case attribute_type::integer: {
      if (const auto updated = InputWidget(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case attribute_type::floating: {
      if (const auto updated = InputWidget(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case attribute_type::boolean: {
      if (const auto updated = InputWidget(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case attribute_type::file: {
      if (auto updated = InputFile(id, value.as_file())) {
        return std::move(updated);
      }
      break;
    }
    case attribute_type::color: {
      if (const auto updated = InputWidget(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case attribute_type::object: {
      if (const auto updated = InputWidget(id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto InputWidget(const c_str id, int value) -> maybe<int>
{
  const scoped::ID scope{id};

  ImGui::SetNextItemWidth(-min_float);
  if (ImGui::DragInt("##InputString[int]", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[int]");
  }

  return nothing;
}

auto InputWidget(const c_str id, float value, const float min, const float max)
    -> maybe<float>
{
  const scoped::ID scope{id};

  ImGui::SetNextItemWidth(-min_float);

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

auto InputStringWithHint(const c_str id,
                         const c_str hint,
                         const std::string& value,
                         const c_str label,
                         const ImGuiInputTextFlags flags,
                         const ImGuiInputTextCallback filter) -> maybe<std::string>
{
  const scoped::ID scope{id};

  std::array<char, 100> buffer;  // NOLINT safely uninitialized
  copy_string_into_buffer(buffer, value);

  if (label) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);

    ImGui::SameLine();
  }
  else {
    ImGui::SetNextItemWidth(-min_float);
  }

  if (ImGui::InputTextWithHint("##InputString[string]",
                               hint ? hint : "",
                               buffer.data(),
                               sizeof buffer,
                               flags,
                               filter)) {
    return create_string_from_buffer(buffer);
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[string]");
  }

  return nothing;
}

auto InputString(const c_str id,
                 const std::string& value,
                 const c_str label,
                 const ImGuiInputTextFlags flags,
                 const ImGuiInputTextCallback filter) -> maybe<std::string>
{
  return InputStringWithHint(id, nullptr, value, label, flags, filter);
}

auto InputWidget(const c_str id, bool value) -> maybe<bool>
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

auto InputWidget(const c_str id, const object_t value) -> maybe<object_t>
{
  const scoped::ID scope{id};

  // TODO
  ImGui::Text("%i", value);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

auto InputWidget(const c_str id, const cen::color value) -> maybe<cen::color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const scoped::ID scope{id};

  auto arr = color_to_array(value);
  if (ImGui::ColorEdit4("##InputString[color]", arr.data(), flags)) {
    return cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

auto InputFile(const c_str id, const std::filesystem::path& value)
    -> maybe<std::filesystem::path>
{
  const scoped::ID scope{id};

  if (ImGui::Button(TAC_ICON_THREE_DOTS)) {
    auto dialog = file_dialog::open_file();
    if (dialog.is_okay()) {
      return dialog.path();
    }
  }

  ImGui::SameLine();

  auto str = value.filename().string();

  ImGui::SetNextItemWidth(-min_float);
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
