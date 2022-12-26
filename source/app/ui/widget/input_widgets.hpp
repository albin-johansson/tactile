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

#pragma once

#include <imgui.h>

#include "core/attribute.hpp"
#include "core/color.hpp"
#include "core/type/maybe.hpp"
#include "core/type/path.hpp"
#include "core/type/string.hpp"

namespace tactile::ui {

[[nodiscard]] auto input_attribute(const char* id, const Attribute& value)
    -> Maybe<Attribute>;

[[nodiscard]] auto input_int(const char* id, int value) -> Maybe<int>;

[[nodiscard]] auto input_float(const char* id, float value, float min = 0, float max = 0)
    -> Maybe<float>;

[[nodiscard]] auto input_string_with_hint(const char* id,
                                          const char* hint,
                                          const String& value,
                                          const char* label = nullptr,
                                          ImGuiInputTextFlags flags = 0,
                                          ImGuiInputTextCallback filter = nullptr)
    -> Maybe<String>;

[[nodiscard]] auto input_string(const char* id,
                                const String& value,
                                const char* label = nullptr,
                                ImGuiInputTextFlags flags = 0,
                                ImGuiInputTextCallback filter = nullptr) -> Maybe<String>;

[[nodiscard]] auto input_bool(const char* id, bool value) -> Maybe<bool>;

[[nodiscard]] auto input_object(const char* id, object_t value) -> Maybe<object_t>;

[[nodiscard]] auto input_color(const char* id, Color value) -> Maybe<Color>;

[[nodiscard]] auto input_file(const char* id, const Path& value) -> Maybe<Path>;

[[nodiscard]] auto input_folder(const char* id, const Path& value) -> Maybe<Path>;

[[nodiscard]] auto ui_attribute_type_combo(AttributeType current_type,
                                           Maybe<AttributeType> excluded_type = nothing)
    -> Maybe<AttributeType>;

}  // namespace tactile::ui
