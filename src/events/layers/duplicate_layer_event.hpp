#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct DuplicateLayerEvent final
{
  layer_id id;
};

/// \} End of group events

}  // namespace Tactile
