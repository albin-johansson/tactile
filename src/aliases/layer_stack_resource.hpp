#pragma once

#include "layer_id.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {

using LayerStackResource = StackResource<16 * sizeof(layer_id)>;

}  // namespace Tactile
