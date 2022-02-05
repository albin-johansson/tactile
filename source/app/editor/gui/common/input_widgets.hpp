#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <centurion.hpp>
#include <imgui.h>

#include "core/attribute_value.hpp"
#include "tactile.hpp"

namespace tactile {

/// General purpose property value input component.
[[nodiscard]] auto Input(c_str id, const attribute_value& value)
    -> maybe<attribute_value>;

[[nodiscard]] auto InputWidget(c_str id, int value) -> maybe<int>;

[[nodiscard]] auto InputWidget(c_str id, float value, float min = 0, float max = 0)
    -> maybe<float>;

[[nodiscard]] auto InputStringWithHint(c_str id,
                                       c_str hint,
                                       const std::string& value,
                                       c_str label = nullptr,
                                       ImGuiInputTextFlags flags = 0,
                                       ImGuiInputTextCallback filter = nullptr)
    -> maybe<std::string>;

[[nodiscard]] auto InputString(c_str id,
                               const std::string& value,
                               c_str label = nullptr,
                               ImGuiInputTextFlags flags = 0,
                               ImGuiInputTextCallback filter = nullptr)
    -> maybe<std::string>;

[[nodiscard]] auto InputWidget(c_str id, bool value) -> maybe<bool>;

[[nodiscard]] auto InputWidget(c_str id, object_t value) -> maybe<object_t>;

[[nodiscard]] auto InputWidget(c_str id, cen::color value) -> maybe<cen::color>;

[[nodiscard]] auto InputFile(c_str id, const std::filesystem::path& value)
    -> maybe<std::filesystem::path>;

}  // namespace tactile
