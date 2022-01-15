#include "move_object_cmd.hpp"

#include "core/components/object.hpp"

namespace Tactile {

MoveObjectCmd::MoveObjectCmd(RegistryRef registry,
                             const ObjectID id,
                             const float oldX,
                             const float oldY,
                             const float newX,
                             const float newY)
    : AObjectCmd{"Move Object", registry, id}
    , mOldX{oldX}
    , mOldY{oldY}
    , mNewX{newX}
    , mNewY{newY}
{}

void MoveObjectCmd::Undo()
{
  auto& object = GetTargetObject();
  object.x = mOldX;
  object.y = mOldY;
}

void MoveObjectCmd::Redo()
{
  auto& object = GetTargetObject();
  object.x = mNewX;
  object.y = mNewY;
}

}  // namespace Tactile
