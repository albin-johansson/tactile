#include "set_tileset_name_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/systems/tileset_system.hpp"

namespace tactile {

SetTilesetNameCmd::SetTilesetNameCmd(registry_ref registry,
                                     const tileset_id id,
                                     std::string name)
    : command_base{"Set Tileset Name"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void SetTilesetNameCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mOldName.value();
}

void SetTilesetNameCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto SetTilesetNameCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetTilesetNameCmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
