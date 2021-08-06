#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetLayerOpacityEvent final
{
  layer_id id;
  float opacity{};
};

/// \} End of group events

}  // namespace Tactile
