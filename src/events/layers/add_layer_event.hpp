#pragma once

#include "core/layer_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddLayerEvent final
{
  LayerType type;
};

/// \} End of group events

}  // namespace Tactile
