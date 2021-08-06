#pragma once

#include "core/map/layers/layer_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddLayerEvent final
{
  LayerType type;
};

/// \} End of group events

}  // namespace Tactile
