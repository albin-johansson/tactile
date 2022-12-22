/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "input_widgets.hpp"

#include <concepts>  // invocable
#include <utility>   // move

#include "buttons.hpp"
#include "core/type/array.hpp"
#include "core/util/buffers.hpp"
#include "core/util/filesystem.hpp"
#include "io/file_dialog.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "scoped.hpp"
#include "tooltips.hpp"
#include "ui/constants.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/icons.hpp"

namespace tactile::ui {
namespace {

template <std::invocable T>
[[nodiscard]] auto input_file_path(const char* id, StringView text, T&& callback)
    -> Maybe<Path>
{
  const Scope scope {id};

  if (button(TAC_ICON_THREE_DOTS)) {
    if (auto result = callback()) {
      return std::move(*result);
    }
  }

  ImGui::SameLine();

  const auto& lang = get_current_language();

  Array<char, 256> buffer;  // NOLINT
  copy_string_into_buffer(buffer, text);

  ImGui::InputTextWithHint("##Path",
                           lang.misc.empty.c_str(),
                           buffer.data(),
                           sizeof buffer,
                           ImGuiInputTextFlags_ReadOnly);

  lazy_tooltip("##PathTooltip", lang.misc.type_path.c_str());

  return nothing;
}

}  // namespace

auto input_attribute(const char* id, const Attribute& value) -> Maybe<Attribute>
{
  switch (value.type()) {
    case AttributeType::String: {
      const auto& lang = get_current_language();
      if (auto updated =
              input_string_with_hint(id, lang.misc.empty.c_str(), value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Int: {
      if (const auto updated = input_int(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float: {
      if (const auto updated = input_float(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case AttributeType::Bool: {
      if (const auto updated = input_bool(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case AttributeType::Path: {
      if (auto updated = input_file(id, value.as_path())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Color: {
      if (const auto updated = input_color(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case AttributeType::Object: {
      if (const auto updated = input_object(id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto input_int(const char* id, int value) -> Maybe<int>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-min_float);
  if (ImGui::DragInt("##Int", &value)) {
    return value;
  }

  const auto& lang = get_current_language();
  lazy_tooltip("##IntTooltip", lang.misc.type_int.c_str());

  return nothing;
}

auto input_float(const char* id, float value, const float min, const float max)
    -> Maybe<float>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-min_float);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat("##Float", &value, min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat("##Float", &value)) {
      return value;
    }
  }

  const auto& lang = get_current_language();
  lazy_tooltip("##FloatTooltip", lang.misc.type_float.c_str());

  return nothing;
}

auto input_string_with_hint(const char* id,
                            const char* hint,
                            const String& value,
                            const char* label,
                            const ImGuiInputTextFlags flags,
                            const ImGuiInputTextCallback filter) -> Maybe<String>
{
  const Scope scope {id};

  Array<char, 128> buffer;  // NOLINT safely uninitialized
  copy_string_into_buffer(buffer, value);

  if (label) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);

    ImGui::SameLine();
  }
  else {
    ImGui::SetNextItemWidth(-min_float);
  }

  if (ImGui::InputTextWithHint("##String",
                               hint ? hint : "",
                               buffer.data(),
                               sizeof buffer,
                               flags,
                               filter)) {
    return create_string_from_buffer(buffer);
  }

  const auto& lang = get_current_language();
  lazy_tooltip("##StringTooltip", lang.misc.type_string.c_str());

  return nothing;
}

auto input_string(const char* id,
                  const String& value,
                  const char* label,
                  const ImGuiInputTextFlags flags,
                  const ImGuiInputTextCallback filter) -> Maybe<String>
{
  return input_string_with_hint(id, nullptr, value, label, flags, filter);
}

auto input_bool(const char* id, bool value) -> Maybe<bool>
{
  const Scope scope {id};

  if (ImGui::Checkbox("##Bool", &value)) {
    return value;
  }

  const auto& lang = get_current_language();
  lazy_tooltip("##BoolTooltip", lang.misc.type_bool.c_str());

  return nothing;
}

auto input_object(const char* id, object_t value) -> Maybe<object_t>
{
  const Scope scope {id};

  // TODO
  ImGui::Text("%i", value);

  const auto& lang = get_current_language();
  lazy_tooltip("##ObjectTooltip", lang.misc.type_object.c_str());

  return nothing;
}

auto input_color(const char* id, const Color value) -> Maybe<Color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const Scope scope {id};

  auto rgba = value.as_float_array();
  if (ImGui::ColorEdit4("##Color", rgba.data(), flags)) {
    return Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
  }

  return nothing;
}

auto input_file(const char* id, const Path& value) -> Maybe<Path>
{
  return input_file_path(id, value.filename().string(), []() -> Maybe<Path> {
    auto dialog = io::FileDialog::open_file();
    return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
  });
}

auto input_folder(const char* id, const Path& value) -> Maybe<Path>
{
  return input_file_path(id,
                         to_canonical(value).value_or(value.string()),
                         []() -> Maybe<Path> {
                           auto dialog = io::FileDialog::open_folder();
                           return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
                         });
}

}  // namespace tactile::ui
