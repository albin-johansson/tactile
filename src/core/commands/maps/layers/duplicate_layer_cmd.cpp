#include "duplicate_layer_cmd.hpp"

#include <format>  // format

#include "core/map_document.hpp"

namespace Tactile {

DuplicateLayerCmd::DuplicateLayerCmd(NotNull<MapDocument*> document,
                                     const layer_id id)
    : MapCommand{document, "Duplicate Layer"}
    , mId{id}
{}

void DuplicateLayerCmd::Undo()
{
  auto& map = GetMap();

  const auto id = mNewId.value();
  auto layer [[maybe_unused]] = map.TakeLayer(id);

  // We need to tell the document that it can safely reuse the layer ID
  map.SetNextLayerId(id);
  mNewId.reset();
}

void DuplicateLayerCmd::Redo()
{
  auto& map = GetMap();

  auto& [id, layer] = map.DuplicateLayer(mId);
  layer->SetName(std::format("{} (Copy)", layer->GetName()));

  mNewId = id;
}

}  // namespace Tactile
