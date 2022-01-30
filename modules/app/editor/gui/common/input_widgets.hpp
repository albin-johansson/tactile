#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <centurion.hpp>
#include <imgui.h>

#include "core/attribute_value.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// General purpose property value input component.
[[nodiscard]] auto Input(CStr id, const attribute_value& value) -> Maybe<attribute_value>;

[[nodiscard]] auto InputWidget(CStr id, int value) -> Maybe<int>;

[[nodiscard]] auto InputWidget(CStr id, float value, float min = 0, float max = 0)
    -> Maybe<float>;

[[nodiscard]] auto InputStringWithHint(CStr id,
                                       CStr hint,
                                       const std::string& value,
                                       CStr label = nullptr,
                                       ImGuiInputTextFlags flags = 0,
                                       ImGuiInputTextCallback filter = nullptr)
    -> Maybe<std::string>;

[[nodiscard]] auto InputString(CStr id,
                               const std::string& value,
                               CStr label = nullptr,
                               ImGuiInputTextFlags flags = 0,
                               ImGuiInputTextCallback filter = nullptr)
    -> Maybe<std::string>;

[[nodiscard]] auto InputWidget(CStr id, bool value) -> Maybe<bool>;

[[nodiscard]] auto InputWidget(CStr id, object_t value) -> Maybe<object_t>;

[[nodiscard]] auto InputWidget(CStr id, cen::color value) -> Maybe<cen::color>;

[[nodiscard]] auto InputFile(CStr id, const std::filesystem::path& value)
    -> Maybe<std::filesystem::path>;

}  // namespace tactile
