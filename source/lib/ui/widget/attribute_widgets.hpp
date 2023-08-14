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

#include "common/type/path.hpp"
#include "tactile/core/contexts/property.hpp"
#include "tactile/core/contexts/color.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "model/services/i18n/strings.hpp"

namespace tactile::ui {

[[nodiscard]] auto push_int_input(const Strings& strings, const char* id, int value)
    -> Maybe<int>;
[[nodiscard]] auto push_int2_input(const Strings& strings, const char* id, Int2 value)
    -> Maybe<Int2>;
[[nodiscard]] auto push_int3_input(const Strings& strings, const char* id, Int3 value)
    -> Maybe<Int3>;
[[nodiscard]] auto push_int4_input(const Strings& strings, const char* id, Int4 value)
    -> Maybe<Int4>;

[[nodiscard]] auto push_float_input(const Strings& strings,
                                    const char* id,
                                    float value,
                                    float min = 0,
                                    float max = 0) -> Maybe<float>;

[[nodiscard]] auto push_float2_input(const Strings& strings,
                                     const char* id,
                                     Float2 value,
                                     float min = 0,
                                     float max = 0) -> Maybe<Float2>;

[[nodiscard]] auto push_float3_input(const Strings& strings,
                                     const char* id,
                                     Float3 value,
                                     float min = 0,
                                     float max = 0) -> Maybe<Float3>;

[[nodiscard]] auto push_float4_input(const Strings& strings,
                                     const char* id,
                                     Float4 value,
                                     float min = 0,
                                     float max = 0) -> Maybe<Float4>;

[[nodiscard]] auto push_string_input_with_hint(const Strings& strings,
                                               const char* id,
                                               const char* hint,
                                               const String& value,
                                               const char* label = nullptr,
                                               ImGuiInputTextFlags flags = 0,
                                               ImGuiInputTextCallback filter = nullptr)
    -> Maybe<String>;

[[nodiscard]] auto push_string_input(const Strings& strings,
                                     const char* id,
                                     const String& value,
                                     const char* label = nullptr,
                                     ImGuiInputTextFlags flags = 0,
                                     ImGuiInputTextCallback filter = nullptr)
    -> Maybe<String>;

[[nodiscard]] auto push_bool_input(const Strings& strings, const char* id, bool value)
    -> Maybe<bool>;

[[nodiscard]] auto push_object_input(const Strings& strings,
                                     const char* id,
                                     ObjectRef value) -> Maybe<ObjectRef>;

[[nodiscard]] auto push_color_input(const char* id, UColor value) -> Maybe<UColor>;

[[nodiscard]] auto push_file_path_input(const Strings& strings,
                                        const char* id,
                                        const Path& value) -> Maybe<Path>;

[[nodiscard]] auto push_directory_path_input(const Strings& strings,
                                             const char* id,
                                             const Path& value) -> Maybe<Path>;

[[nodiscard]] auto push_attribute_input(const Strings& strings,
                                        const char* id,
                                        const Property& value) -> Maybe<Property>;

[[nodiscard]] auto push_attribute_type_combo(const Strings& strings,
                                             PropertyType current_type,
                                             Maybe<PropertyType> excluded_type = {})
    -> Maybe<PropertyType>;

}  // namespace tactile::ui
