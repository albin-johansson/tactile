#include "move_object_cmd.hpp"

#include <cassert>  // assert

#include "core/components/object.hpp"
#include "core/systems/object_system.hpp"

namespace Tactile {

MoveObjectCmd::MoveObjectCmd(Ref<entt::registry> registry,
                             const ObjectID id,
                             const float oldX,
                             const float oldY,
                             const float newX,
                             const float newY)
    : ACommand{"Move Object"}
    , mRegistry{registry}
    , mObject{id}
    , mOldX{oldX}
    , mOldY{oldY}
    , mNewX{newX}
    , mNewY{newY}
{}

void MoveObjectCmd::Undo()
{
  SetObjectPosition(mOldX, mOldY);
}

void MoveObjectCmd::Redo()
{
  SetObjectPosition(mNewX, mNewY);
}

void MoveObjectCmd::SetObjectPosition(const float x, const float y)
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindObject(registry, mObject);
  assert(entity != entt::null);

  auto& object = registry.get<Object>(entity);
  object.x = x;
  object.y = y;
}

}  // namespace Tactile
