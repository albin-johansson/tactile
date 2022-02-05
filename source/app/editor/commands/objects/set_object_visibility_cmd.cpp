#include "set_object_visibility_cmd.hpp"

#include "core/components/object.hpp"

namespace tactile {

set_object_visibility_cmd::set_object_visibility_cmd(registry_ref registry,
                                                     const object_id id,
                                                     const bool visible)
    : object_cmd{"Set Object Visibility", registry, id}
    , mVisible{visible}
{}

void set_object_visibility_cmd::undo()
{
  auto& object = target_object();
  object.visible = mPreviousVisibility.value();
}

void set_object_visibility_cmd::redo()
{
  auto& object = target_object();
  mPreviousVisibility = object.visible;
  object.visible = mVisible;
}

}  // namespace tactile
