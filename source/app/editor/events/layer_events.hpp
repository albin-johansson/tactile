#pragma once

#include <string>  // string

#include "core/layer_type.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct AddLayerEvent final
{
  layer_type type;
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

struct OpenRenameLayerDialogEvent final
{
  layer_id id;
};

struct RenameLayerEvent final
{
  layer_id id;
  std::string name;
};

/// \} End of group events

}  // namespace tactile
