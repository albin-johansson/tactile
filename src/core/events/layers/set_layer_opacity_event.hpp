#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

struct SetLayerOpacityEvent final
{
  layer_id id;
  float opacity{};
};

}  // namespace Tactile
