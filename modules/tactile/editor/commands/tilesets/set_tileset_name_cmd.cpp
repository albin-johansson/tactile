#include "set_tileset_name_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/systems/tileset_system.hpp"

namespace tactile {

SetTilesetNameCmd::SetTilesetNameCmd(RegistryRef registry,
                                     const TilesetID id,
                                     std::string name)
    : ACommand{"Set Tileset Name"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void SetTilesetNameCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<attribute_context>(entity);
  context.name = mOldName.value();
}

void SetTilesetNameCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<attribute_context>(entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto SetTilesetNameCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetTilesetNameCmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
