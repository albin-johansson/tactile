// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/core/meta/color.hpp"

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
