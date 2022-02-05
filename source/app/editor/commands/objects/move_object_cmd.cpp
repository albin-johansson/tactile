#include "move_object_cmd.hpp"

#include "core/components/object.hpp"

namespace tactile {

move_object_cmd::move_object_cmd(registry_ref registry,
                                 const object_id id,
                                 const float oldX,
                                 const float oldY,
                                 const float newX,
                                 const float newY)
    : object_cmd{"Move Object", registry, id}
    , mOldX{oldX}
    , mOldY{oldY}
    , mNewX{newX}
    , mNewY{newY}
{}

void move_object_cmd::undo()
{
  auto& object = target_object();
  object.x = mOldX;
  object.y = mOldY;
}

void move_object_cmd::redo()
{
  auto& object = target_object();
  object.x = mNewX;
  object.y = mNewY;
}

}  // namespace tactile
