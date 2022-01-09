#include "update_component_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

UpdateComponentCmd::UpdateComponentCmd(RegistryRef registry,
                                       const ContextID contextId,
                                       const ComponentID componentId,
                                       std::string attribute,
                                       PropertyValue value)
    : ACommand{"Update Component Value"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void UpdateComponentCmd::Undo()
{
  auto& registry = mRegistry.get();

  Sys::UpdateComponent(registry,
                       mContextId,
                       mComponentId,
                       mAttributeName,
                       mPreviousValue.value());

  mPreviousValue.reset();
}

void UpdateComponentCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      Sys::GetComponentAttribute(registry, mContextId, mComponentId, mAttributeName);
  Sys::UpdateComponent(registry, mContextId, mComponentId, mAttributeName, mUpdatedValue);
}

auto UpdateComponentCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const UpdateComponentCmd&>(cmd);

    const bool targetsSameAttribute = mContextId == other.mContextId &&
                                      mComponentId == other.mComponentId &&
                                      mAttributeName == other.mAttributeName;
    if (targetsSameAttribute) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile