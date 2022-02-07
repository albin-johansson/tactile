#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <centurion.hpp>
#include <imgui.h>

#include "core/attribute.hpp"
#include "tactile.hpp"

namespace tactile {

[[nodiscard]] auto input_attribute(const char* id, const attribute_value& value)
    -> maybe<attribute_value>;

[[nodiscard]] auto input_int(const char* id, int value) -> maybe<int>;

[[nodiscard]] auto input_float(const char* id, float value, float min = 0, float max = 0)
    -> maybe<float>;

[[nodiscard]] auto input_string_with_hint(const char* id,
                                          const char* hint,
                                          const std::string& value,
                                          const char* label = nullptr,
                                          ImGuiInputTextFlags flags = 0,
                                          ImGuiInputTextCallback filter = nullptr)
    -> maybe<std::string>;

[[nodiscard]] auto input_string(const char* id,
                                const std::string& value,
                                const char* label = nullptr,
                                ImGuiInputTextFlags flags = 0,
                                ImGuiInputTextCallback filter = nullptr)
    -> maybe<std::string>;

[[nodiscard]] auto input_bool(const char* id, bool value) -> maybe<bool>;

[[nodiscard]] auto input_object(const char* id, object_t value) -> maybe<object_t>;

[[nodiscard]] auto input_color(const char* id, cen::color value) -> maybe<cen::color>;

[[nodiscard]] auto input_file(const char* id, const std::filesystem::path& value)
    -> maybe<std::filesystem::path>;

}  // namespace tactile
