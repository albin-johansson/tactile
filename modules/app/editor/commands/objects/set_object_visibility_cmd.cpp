#include "set_object_visibility_cmd.hpp"

#include "core/components/object.hpp"

namespace tactile {

SetObjectVisibilityCmd::SetObjectVisibilityCmd(RegistryRef registry,
                                               const ObjectID id,
                                               const bool visible)
    : AObjectCmd{"Set Object Visibility", registry, id}
    , mVisible{visible}
{}

void SetObjectVisibilityCmd::Undo()
{
  auto& object = GetTargetObject();
  object.visible = mPreviousVisibility.value();
}

void SetObjectVisibilityCmd::Redo()
{
  auto& object = GetTargetObject();
  mPreviousVisibility = object.visible;
  object.visible = mVisible;
}

}  // namespace tactile
