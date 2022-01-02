#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <centurion.hpp>
#include <imgui.h>
#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile {

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

[[nodiscard]] auto InputWidget(CStr id, ObjectRef value) -> Maybe<ObjectRef>;

[[nodiscard]] auto InputWidget(CStr id, cen::Color value) -> Maybe<cen::Color>;

[[nodiscard]] auto InputFile(CStr id, const std::filesystem::path& value)
    -> Maybe<std::filesystem::path>;

}  // namespace Tactile
