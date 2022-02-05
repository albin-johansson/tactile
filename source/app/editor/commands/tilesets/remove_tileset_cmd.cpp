#include "remove_tileset_cmd.hpp"

#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

remove_tileset_cmd::remove_tileset_cmd(registry_ref registry, const tileset_id id)
    : command_base{"Remove Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
{}

void remove_tileset_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_tileset(registry, mSnapshot.value());

  auto& active = registry.ctx<comp::active_tileset>();
  active.entity = sys::find_tileset(registry, mTilesetId);
}

void remove_tileset_cmd::redo()
{
  const auto entity = sys::find_tileset(mRegistry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(mRegistry, entity);
  sys::remove_tileset(mRegistry, mTilesetId);
}

}  // namespace tactile
