#pragma once

#include "aliases/layer_id.hpp"
#include "core/layer_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddLayerEvent final
{
  LayerType type;
};

struct RemoveLayerEvent final
{
  layer_id id;
};

struct DuplicateLayerEvent final
{
  layer_id id;
};

struct SelectLayerEvent final
{
  layer_id id;
};

struct MoveLayerUpEvent final
{
  layer_id id;
};

struct MoveLayerDownEvent final
{
  layer_id id;
};

struct SetLayerOpacityEvent final
{
  layer_id id;
  float opacity{};
};

struct SetLayerVisibleEvent final
{
  layer_id id;
  bool visible{};
};

struct ShowLayerPropertiesEvent final
{
  layer_id id;
};

/// \} End of group events

}  // namespace Tactile
