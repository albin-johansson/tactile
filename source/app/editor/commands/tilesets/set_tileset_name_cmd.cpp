#include "set_tileset_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

set_tileset_name_cmd::set_tileset_name_cmd(registry_ref registry,
                                           const tileset_id id,
                                           std::string name)
    : command_base{"Set Tileset Name"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void set_tileset_name_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mOldName.value();
}

void set_tileset_name_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto set_tileset_name_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_tileset_name_cmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
