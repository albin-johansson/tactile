#include "add_tileset_cmd.hpp"

#include <utility>  // move

#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

add_tileset_cmd::add_tileset_cmd(registry_ref registry,
                                 comp::texture texture,
                                 const int32 tileWidth,
                                 const int32 tileHeight)
    : command_base{"Add Tileset"}
    , mRegistry{registry}
    , mTexture{std::move(texture)}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
{}

void add_tileset_cmd::undo()
{
  auto& registry = mRegistry.get();
  const auto id = mTilesetId.value();

  const auto entity = sys::find_tileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(registry, entity);
  sys::remove_tileset(registry, id);
}

void add_tileset_cmd::redo()
{
  auto& registry = mRegistry.get();
  if (!mSnapshot) {
    const auto entity = sys::make_tileset(registry, mTexture, mTileWidth, mTileHeight);
    const auto& tileset = registry.get<comp::tileset>(entity);
    mTilesetId = tileset.id;
  }
  else {
    sys::restore_tileset(registry, *mSnapshot);
  }

  auto& active = registry.ctx<comp::active_tileset>();
  active.entity = sys::find_tileset(registry, mTilesetId.value());
}

}  // namespace tactile
