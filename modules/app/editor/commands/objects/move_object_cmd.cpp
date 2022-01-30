#include "move_object_cmd.hpp"

#include "core/components/object.hpp"

namespace tactile {

MoveObjectCmd::MoveObjectCmd(RegistryRef registry,
                             const object_id id,
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

}  // namespace tactile
