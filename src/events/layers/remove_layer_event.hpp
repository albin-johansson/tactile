#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct RemoveLayerEvent final
{
  layer_id id;
};

/// \} End of group events

}  // namespace Tactile
