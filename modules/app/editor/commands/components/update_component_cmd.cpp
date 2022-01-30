#include "update_component_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

UpdateComponentCmd::UpdateComponentCmd(RegistryRef registry,
                                       const context_id contextId,
                                       const component_id componentId,
                                       std::string attribute,
                                       attribute_value value)
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

  sys::update_component(registry,
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
      sys::get_component_attribute(registry, mContextId, mComponentId, mAttributeName);
  sys::update_component(registry,
                        mContextId,
                        mComponentId,
                        mAttributeName,
                        mUpdatedValue);
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

}  // namespace tactile