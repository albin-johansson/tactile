#pragma once

#include "aliases/layer_id.hpp"

namespace Tactile {

struct SetLayerVisibleEvent final
{
  layer_id id;
  bool visible{};
};

}  // namespace Tactile
