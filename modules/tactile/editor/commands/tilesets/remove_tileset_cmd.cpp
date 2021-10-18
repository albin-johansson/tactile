#include "remove_tileset_cmd.hpp"

#include <cassert>  // assert

#include "tactile/core/systems/tileset_system.hpp"

namespace Tactile {

RemoveTilesetCmd::RemoveTilesetCmd(Ref<entt::registry> registry, const TilesetID id)
    : ACommand{"Remove Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
{}

void RemoveTilesetCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RestoreTileset(registry, mSnapshot.value());

  auto& active = registry.ctx<ActiveTileset>();
  active.entity = Sys::FindTileset(registry, mTilesetId);
}

void RemoveTilesetCmd::Redo()
{
  const auto entity = Sys::FindTileset(mRegistry, mTilesetId);
  assert(entity != entt::null);

  mSnapshot = Sys::CopyTileset(mRegistry, entity);
  Sys::RemoveTileset(mRegistry, mTilesetId);
}

}  // namespace Tactile
