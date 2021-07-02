#include "add_layer_cmd.hpp"

#include <utility>  // move

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

AddLayerCmd::AddLayerCmd(NotNull<MapDocument*> document,
                         SharedLayer layer,
                         const layer_id id)
    : MapCommand{document, "Add Layer"}
    , mLayer{std::move(layer)}
    , mId{id}
{
  if (!mLayer)
  {
    throw TactileError{"Cannot create command from null layer!"};
  }
}

void AddLayerCmd::Undo()
{
  auto& map = GetMap();

  // We already have a shared pointer to the layer in question
  auto layer [[maybe_unused]] = map.TakeLayer(mId);
}

void AddLayerCmd::Redo()
{
  auto& map = GetMap();
  map.AddLayer(mId, mLayer);
}

}  // namespace Tactile
