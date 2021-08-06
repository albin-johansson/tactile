#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetLayerVisibleEvent final
{
  layer_id id;
  bool visible{};
};

/// \} End of group events

}  // namespace Tactile
