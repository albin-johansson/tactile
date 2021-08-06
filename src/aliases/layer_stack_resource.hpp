#pragma once

#include "layer_id.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {

template <typename T = layer_id>
using LayerStackResource = StackResource<T, 16>;

}  // namespace Tactile
