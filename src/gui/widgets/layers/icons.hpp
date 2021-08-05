#pragma once

#include <string_view>  // string_view

#include "core/map/layers/layer_type.hpp"

namespace Tactile {

[[nodiscard]] auto GetIcon(LayerType type) noexcept -> std::string_view;

}  // namespace Tactile
