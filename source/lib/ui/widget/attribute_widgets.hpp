/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"

namespace tactile::ui {

[[nodiscard]] auto ui_int_input(const char* id, int value) -> Maybe<int>;
[[nodiscard]] auto ui_int2_input(const char* id, Int2 value) -> Maybe<Int2>;
[[nodiscard]] auto ui_int3_input(const char* id, Int3 value) -> Maybe<Int3>;
[[nodiscard]] auto ui_int4_input(const char* id, Int4 value) -> Maybe<Int4>;

[[nodiscard]] auto ui_float_input(const char* id,
                                  float value,
                                  float min = 0,
                                  float max = 0) -> Maybe<float>;

[[nodiscard]] auto ui_float2_input(const char* id,
                                   Float2 value,
                                   float min = 0,
                                   float max = 0) -> Maybe<Float2>;

[[nodiscard]] auto ui_float3_input(const char* id,
                                   Float3 value,
                                   float min = 0,
                                   float max = 0) -> Maybe<Float3>;

[[nodiscard]] auto ui_float4_input(const char* id,
                                   Float4 value,
                                   float min = 0,
                                   float max = 0) -> Maybe<Float4>;

[[nodiscard]] auto ui_string_input_with_hint(const char* id,
                                             const char* hint,
                                             const String& value,
                                             const char* label = nullptr,
                                             ImGuiInputTextFlags flags = 0,
                                             ImGuiInputTextCallback filter = nullptr)
    -> Maybe<String>;

[[nodiscard]] auto ui_string_input(const char* id,
                                   const String& value,
                                   const char* label = nullptr,
                                   ImGuiInputTextFlags flags = 0,
                                   ImGuiInputTextCallback filter = nullptr)
    -> Maybe<String>;

[[nodiscard]] auto ui_bool_input(const char* id, bool value) -> Maybe<bool>;

[[nodiscard]] auto ui_object_input(const char* id, ObjectRef value) -> Maybe<ObjectRef>;

[[nodiscard]] auto ui_color_input(const char* id, Color value) -> Maybe<Color>;

[[nodiscard]] auto ui_file_path_input(const char* id, const Path& value) -> Maybe<Path>;

[[nodiscard]] auto ui_directory_path_input(const char* id, const Path& value)
    -> Maybe<Path>;

[[nodiscard]] auto ui_attribute_input(const char* id, const Attribute& value)
    -> Maybe<Attribute>;

[[nodiscard]] auto ui_attribute_type_combo(AttributeType current_type,
                                           Maybe<AttributeType> excluded_type = nothing)
    -> Maybe<AttributeType>;

}  // namespace tactile::ui
