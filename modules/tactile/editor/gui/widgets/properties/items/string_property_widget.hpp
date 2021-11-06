#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto StringPropertyWidget(const PropertyValue& property,
                                        ImGuiInputTextFlags flags = 0,
                                        ImGuiInputTextCallback filter = nullptr)
    -> Maybe<std::string>;

}  // namespace Tactile
